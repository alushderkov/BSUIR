import uuid
from typing import Union
from fastapi import FastAPI
from starlette.responses import JSONResponse
from pydantic import BaseModel
from os import getenv, system


class Image(BaseModel):
    uuid: uuid.UUID
    link: str


app = FastAPI()


@app.post("/run_check")
async def run_check(image: Union[Image]):
    error = False
    detail = 'Success'

    cmd = ' '.join([
        'trivy image',
        '--server http://localhost:{port}'.format(port=getenv('TRIVY_PORT')),
        '--token {token}'.format(token=getenv('TRIVY_TOKEN')),
        image.link,
    ])

    cmd_json = ' '.join([
        cmd,
        '--format json',
        '-o /opt/reports/tmp/{uuid}/report.json'.format(uuid=image.uuid),
    ])

    cmd_html = ' '.join([
        cmd,
        '--format template',
        '--template "@/usr/local/share/trivy/templates/html.tpl"',
        '-o /opt/reports/tmp/{uuid}/report.html'.format(uuid=image.uuid),
    ])

    try:
        print('run:', image.uuid)
        system(cmd_json)
        system(cmd_html)
    except Exception as e:
        error = True
        detail = str(e)
        print('error:', detail)

    return JSONResponse({'error': error, 'detail': detail})
