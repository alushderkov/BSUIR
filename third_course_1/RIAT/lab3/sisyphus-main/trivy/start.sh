#!/bin/sh

uvicorn main:app --host 0.0.0.0 --port 8080 --reload & trivy server --listen 0.0.0.0:$TRIVY_PORT --token $TRIVY_TOKEN
