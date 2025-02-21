#!/bin/sh

cron

python manage.py db_health_check
python manage.py migrate --no-input
python manage.py collectstatic --no-input --clear
python manage.py create_superuser --no-input
python manage.py crontab add
python manage.py runserver 0.0.0.0:8000
