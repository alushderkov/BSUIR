from django.db import connections
from django.db.utils import OperationalError
from django.core.management import BaseCommand
from time import sleep


class Command(BaseCommand):
    def handle(self, *args, **options):
        self.stdout.write('Ожидание подключения к БД')

        db_conn = None
        while not db_conn:
            try:
                db_conn = connections['default']
            except OperationalError:
                self.stdout.write('БД недоступна, ожидаем 1 секунду...')
                sleep(1)

        self.stdout.write(self.style.SUCCESS('База данных доступна'))