import json

import logging
import boto3
from botocore.exceptions import ClientError


def jsonCreation(name, limit, range):
    jsonObj = {} #creation of JSON object
    jsonObj['data'] = []
    jsonObj['data'].append({'name': name, 'limit': limit, 'range': range})

    return jsonObj

def upload_file(file_name, bucket, object_name=None):
    if object_name is None:
        object_name = file_name

    # Upload the file
    s3_client = boto3.client('s3')
    try:
        response = s3_client.upload_file(file_name, bucket, object_name)

    except ClientError as e:
        logging.error(e)
        return False
    return True

name = input('Name: ')
limit = input('Max Spending Limit: ')
range = input('Over what time range: ')


jsonObj = jsonCreation(name, limit, range)

with open('newFile.json', 'w') as jsonFile:
    json.dump(jsonObj, jsonFile)


upload_file('newFile.json', 'money-mug', )
