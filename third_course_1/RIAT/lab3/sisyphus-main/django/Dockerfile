FROM python:latest

WORKDIR /opt/app

ENV PYTHONDONTWRITEBYTECODE 1
ENV PYTHONUNBUFFERED 1

COPY . .

RUN pip install --upgrade pip
RUN pip install -r requirements.txt

RUN mkdir /opt/reports && mkdir /opt/reports/tmp
RUN apt-get update && apt-get install -y cron

CMD ["sh", "/opt/app/start.sh"]