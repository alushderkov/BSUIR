from json import loads, dumps

from django.contrib import admin, messages
from django.utils.html import format_html
from django.views.decorators.cache import never_cache

from request import consts
from request.models import Request
from django_object_actions import DjangoObjectActions, action


# Register your models here.
@admin.register(Request)
class RequestAdmin(DjangoObjectActions, admin.ModelAdmin):
    list_display = ('uuid', 'created_by', 'type', 'link', 'status', 'reviewed_by')
    ordering = ('-created_at',)
    list_filter = ('status', 'type')

    def get_queryset(self, request):
        base = super().get_queryset(request)

        if not (request.user.is_superuser or request.user.has_perm('request.security')):
            base = base.filter(created_by=request.user)

        return base

    def has_change_permission(self, request, obj=None):
        return False

    def has_delete_permission(self, request, obj=None):
        return False

    def save_model(self, request, obj, form, change):
        user = request.user
        obj = form.save(commit=False)
        obj.created_by = user
        obj.save()
        form.save_m2m()
        return obj

    @action(label='Approve')
    def approve(self, request, obj):
        try:
            if obj.type == consts.TYPE_DOCKER:
                obj.save_image(request.user)
            else:
                raise Exception('Save method for {} is not found'.format(obj.get_type_display()))
            self.message_user(request, 'Approve {}'.format(obj))
        except Exception as e:
            self.message_user(request, str(e), level=messages.ERROR)

    @action(label='Reject')
    def reject(self, request, obj):
        obj.status = consts.STATUS_REJECTED
        obj.reviewed_by = request.user
        obj.save()

        self.message_user(request, 'Reject {}'.format(obj))

    change_actions = ['approve', 'reject']

    def get_change_actions(self, request, object_id, form_url):
        req = Request.objects.get(pk=object_id)

        if (
                req.status == consts.STATUS_PROCESSED and
                (request.user.is_superuser or request.user.has_perm('request.security'))
        ):
            return ['approve', 'reject']
        return []

    def get_fields(self, request, obj=None):
        fields = ['type', 'link', 'reason']

        if obj:
            fields.insert(0, 'uuid')
            fields.append('status')

            if obj.status == consts.STATUS_PROCESSED:
                fields.append('repo_digests')

            if obj.has_html:
                fields.append('html_report')

            if obj.has_json:
                fields.append('fast_report')

            if obj.status == consts.STATUS_APPROVED:
                fields.append('run_info')

                if obj.type == consts.TYPE_DOCKER:
                    fields.append('docker_image')

            if obj.status > consts.STATUS_PROCESSED:
                fields.append('reviewed_by')

            fields += ['created_by', 'created_at']

        return fields

    def html_report(self, obj):
        return format_html('<a href="/media/{}/report.html" target="_blank">Download</a>', obj.pk)

    def docker_image(self, obj):
        return format_html('<a href="/media/{}/container.tar" download>Download</a>', obj.pk)

    def fast_report(self, obj):
        json_data = loads(obj.short_report_info)
        return format_html("""<pre>{}</pre>""", dumps(json_data, indent=4))

