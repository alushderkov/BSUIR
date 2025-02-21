import uuid
from django.db import models
from django.utils.html import format_html
from requests import post

from project.settings import AUTH_USER_MODEL
from request import consts


class Request(models.Model):
    TYPE_CHOICES = (
        (consts.TYPE_DOCKER, 'Docker Image'),
        # (consts.TYPE_JAR, 'JAR File'),
    )

    STATUS_CHOICES = (
        (consts.STATUS_CREATED, 'Created'),
        (consts.STATUS_IN_PROCESSING, 'In processing'),
        (consts.STATUS_PROCESSED, 'Processed'),
        (consts.STATUS_REJECTED, 'Rejected'),
        (consts.STATUS_APPROVED, 'Approved'),
        (consts.STATUS_INVALID, 'Invalid'),
    )

    uuid = models.UUIDField(primary_key=True, default=uuid.uuid4, editable=False)
    created_by = models.ForeignKey(AUTH_USER_MODEL, on_delete=models.CASCADE, editable=False)
    created_at = models.DateTimeField(auto_now_add=True, editable=False)

    type = models.IntegerField(choices=TYPE_CHOICES)
    link = models.CharField(max_length=500)
    reason = models.TextField(blank=True, null=True)

    status = models.IntegerField(choices=STATUS_CHOICES, default=consts.STATUS_CREATED, editable=False)
    reviewed_by = models.ForeignKey(
        AUTH_USER_MODEL,
        on_delete=models.CASCADE,
        editable=False,
        null=True,
        blank=True,
        related_name='requests_approved',
    )

    has_json = models.BooleanField(editable=False, default=False)
    has_html = models.BooleanField(editable=False, default=False)
    repo_digests = models.TextField(editable=False)
    short_report_info = models.TextField(editable=False)

    def __str__(self):
        return '{link} ({uuid})'.format(link=self.link, uuid=self.uuid)

    class Meta:
        permissions = (
            ('security', 'Can approve / reject request'),
        )

    def save_image(self, user):
        url = 'http://docker:8080/run_save'
        data = {
            'uuid': str(self.uuid),
            'link': self.repo_digests,
        }

        resp = post(url, json=data)

        if resp.status_code != 200:
            raise Exception('Status of saving server - {}'.format(resp.status_code))

        resp_json = resp.json()
        if resp_json.get('error'):
            raise Exception(resp_json.get('detail'))

        self.status = consts.STATUS_APPROVED
        self.reviewed_by = user
        self.save()

    @property
    def run_info(self):
        if self.type == consts.TYPE_DOCKER:
            return format_html('''<ul>
<li><pre>curl -o {uuid}.tar.gz http://localhost/media/{uuid}/container.tar</pre></li>
<li><pre>docker load < {uuid}.tar.gz</pre></li>
<li><pre>rm {uuid}.tar.gz</pre></li>
</ul>''', uuid=self.uuid)

        return 'Run info is empty :('

