from django.contrib.auth import get_user_model
from django.core.management.base import BaseCommand
from os import environ


class Command(BaseCommand):
    def add_arguments(self, parser):
        parser.add_argument('--username')
        parser.add_argument('--email')
        parser.add_argument('--password')
        parser.add_argument('--no-input', action='store_true')

    def handle(self, *args, **options):
        User = get_user_model()

        if options['no_input']:
            options['username'] = environ.get('SUPERUSER_USERNAME')
            options['email'] = environ.get('SUPERUSER_EMAIL')
            options['password'] = environ.get('SUPERUSER_PASSWORD')

        if options['username'] and options['email'] and options['password']:
            if not User.objects.filter(username=options['username']).exists():
                User.objects.create_superuser(
                    username=options['username'],
                    email=options['email'],
                    password=options['password'],
                )
                self.stdout.write(self.style.SUCCESS(f'Пользователь {options["username"]} создан'))
            else:
                self.stdout.write(self.style.SUCCESS(f'Пользователь {options["username"]} уже был создан ранее'))
        else:
            self.stdout.write(self.style.SUCCESS('Параметры для создания пользователя не предоставлены'))