from shutil import move
from os import path, makedirs, removedirs, listdir
from requests import post
from request.models import Request
from request import consts
from json import load, dumps


_reports_dir = '/opt/reports'
_reports_dir_tmp = _reports_dir + '/tmp'


def run_check():
    print('Running run_check...')
    try:
        for obj in Request.objects.filter(status=consts.STATUS_CREATED).all():
            url = None
            data = dict()

            report_dir = path.join(_reports_dir_tmp, str(obj.uuid))
            if not path.exists(report_dir):
                makedirs(report_dir)

            if obj.type == consts.TYPE_DOCKER:
                url = 'http://trivy:8080/run_check'
                data['uuid'] = str(obj.uuid)
                data['link'] = obj.link

            if url is None:
                obj.status = consts.STATUS_INVALID
                obj.save()
                raise Exception('System for check is not found')

            print('{} -> {}'.format(url, str(obj.uuid)))
            resp = post(url, json=data)

            if resp.status_code != 200:
                raise Exception('Status of checking server - {}'.format(resp.status_code))

            resp_json = resp.json()
            if resp_json.get('error'):
                raise Exception(resp_json.get('detail'))

            obj.status = consts.STATUS_IN_PROCESSING
            obj.save()
    except Exception as e:
        print('Error: {}'.format(str(e)))


def check_result():
    print('Running check_result...')
    try:
        directories = [d for d in listdir(_reports_dir_tmp) if path.isdir(path.join(_reports_dir_tmp, d))]
        for directory in directories:
            print('Checking directory: {}'.format(directory))
            directory_path_tmp = path.join(_reports_dir_tmp, directory)

            request = Request.objects.filter(pk=directory).first()

            if request is None:
                continue

            directory_path = path.join(_reports_dir, directory)
            if not path.exists(directory_path):
                makedirs(directory_path)

            files = [f for f in listdir(directory_path_tmp) if path.isfile(path.join(directory_path_tmp, f))]
            for file in files:
                print('Checking file: {}'.format(file))
                file_path_tmp = path.join(directory_path_tmp, file)
                file_path = path.join(directory_path, file)

                extension = file.split('.')[-1]

                if extension == 'json':
                    request.has_json = True

                    with open(file_path_tmp) as f:
                        info = load(f)
                        request.repo_digests = info.get('Metadata').get('RepoDigests')[0]

                        severities = dict()
                        for result in info.get('Results'):
                            if 'Vulnerabilities' in result:
                                for vulnerability in result.get('Vulnerabilities'):
                                    severity = vulnerability.get('Severity')
                                    if severity not in severities:
                                        severities[severity] = 1
                                    else:
                                        severities[severity] += 1
                        request.short_report_info = dumps(severities)
                elif extension == 'html':
                    request.has_html = True

                try:
                    move(file_path_tmp, file_path)
                    request.save()
                except Exception as e:
                    print('Error: {}'.format(str(e)))

            if request.has_json and request.has_html:
                try:
                    removedirs(directory_path_tmp)
                    request.status = consts.STATUS_PROCESSED
                    request.save()
                except Exception as e:
                    print('Error: {}'.format(str(e)))
    except Exception as e:
        print('Error: {}'.format(str(e)))