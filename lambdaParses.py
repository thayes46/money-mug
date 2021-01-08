import pymysql
import json
import boto3

def lambda_handler(event, context):

    def handler():
        cursor = connection.cursor()
        cursor.execute('SELECT * from Transactions')

        rows = cursor.fetchall()
        transId = rows[0]
        total = rows[1]
        transType = rows[2]

        return {
            transId, transType, total
        }

    s3 = boto3.resource('s3')

    bucket = 'money-mug'
    key = 'newFile.json'

    endpoint = 'database-112.crz5qomac1oy.us-east-2.rds.amazonaws.com'
    username = 'testUser'
    password = 'confirm1'
    database_name = 'database-112'

    spent = 0
    percent = 0

    connection = pymysql.connect(user=username, password=password,
        database=database_name, port=3306, endpoint)


    try:
        response = s3.get_objeect(Bucket=bucket, Key=key)

        text = response["Body"].read().decode()
        data = json.loads(text)

        print(data)

        items = data['data']
        username = items['username']
        userLimit = items['limit']
        userRange = items['range']
        category = items['category']
        for i in items:
            print(i['category'])
        return {
        uername, userLimit, userRange, category
        }

    except: Exception as e:
        print(e)
        raise e

    handler()

    for row in total:
        spent = remaining + rows

    remaining = userLimit - spent

    percent = int((remaining/userLimit)*100)

    if percent > 100:
        return output = 0
    else: continue

    return{
        'statusCode': 300,
        'body': output
    }
