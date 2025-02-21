Запуск клиента (на примере обаза ubuntu/apache2:latest):
```shell
trivy image  --server http://localhost:4030 --token dummy -f json -o results.json ubuntu/apache2:latest
trivy image  --server http://localhost:4030 --token dummy --format template --template "@./contrib/html.tpl" -o ./1-rep.html ubuntu/apache2:latest
```
