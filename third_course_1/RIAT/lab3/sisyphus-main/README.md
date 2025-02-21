# Sisyphus

### Установка
```bash
docker compose up -d --build
```

### Контейнеры
1. Django - клиентское приложение
2. Trivy - сканер docker-образов
   1. Trivy - сканер docker-образов
   2. FastAPI - микросервис для запуска Trivy

~~~
Для базового доступа следует воспользоваться логином и паролем из django/.env
~~~