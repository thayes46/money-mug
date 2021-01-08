import json
import pymysql 

endpoint = 'database-112.crz5qomac1oy.us-east-2.rds.amazonaws.com'
username = 'testUser'
password = 'confirm1'
database_name = 'Transactions'

connection = pymysql.connect(host = endpoint, user=username, passwd=password, db = database_name, autocommit=True)
def data(transact):
    cursor = connection.cursor()
    num_rows = cursor.execute('SELECT * from TRANSACTIONS')

    rows = cursor.fetchall()
        
    id_number = int(rows[num_rows-1][0])+ 1
    for trans in transact["transactions"]:
        total = trans["amount"]
        date = str(trans["date"])
        date = "\"" + date + "\""

        sqlQuery = "INSERT INTO TRANSACTIONS (transaction_id, transaction_date, total, transaction_type) VALUES(" + str(id_number) + ", " + date + ", " + str(total) + ", " + str(1) +")"

        cursor.execute(sqlQuery)
        id_number += 1

connection.close()
