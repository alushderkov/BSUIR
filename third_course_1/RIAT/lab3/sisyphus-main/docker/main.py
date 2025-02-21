import uuid
from typing import Union
from fastapi import FastAPI
from starlette.responses import JSONResponse
from pydantic import BaseModel
from subprocess import Popen, PIPE


class Image(BaseModel):
    uuid: uuid.UUID
    link: str


app = FastAPI()


@app.post("/run_save")
async def run_save(image: Union[Image]):
    error = False
    detail = 'Success'

    cmd_pull = ' '.join([
        'docker pull',
        image.link,
    ])

    cmd_save = ' '.join([
        'docker save',
        '-o /opt/reports/{}/container.tar'.format(image.uuid),
        image.link,
    ])

    try:
        print('run:', image.uuid)

        print(cmd_pull)
        process_pull = Popen(cmd_pull, shell=True, stdout=PIPE)
        process_pull.wait()

        print(cmd_save)
        process_save = Popen(cmd_save, shell=True, stdout=PIPE)
        process_save.wait()
    except Exception as e:
        error = True
        detail = str(e)
        print('error:', detail)

    return JSONResponse({'error': error, 'detail': detail})
