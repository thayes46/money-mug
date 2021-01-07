import cv2
import pytesseract
import pymysql 

# Finds date in string
def find_date(input_string):
	import re
	date = ""
  	dateList = re.search("\d{2}(?P<sep>[-/])\d{2}(?P=sep)\d{4}", input_string)
  	if dateList != None:
		date = dateList.group()
		return date
  	return date

# Find total amount due -> questionable, finds largest decimal and assumes its the total
def find_total(input_string):
	import re
  	dec = re.findall("\d+\.\d+", input_string)

  	if len(dec) == 0:
  		return None
  	else:
    	return float(max(dec))

# Finds name of store using a pre defined list of store names
business_names = ["STARBUCKS", "DUNKIN DONUTS"] 
def find_name(input_string):
	temp = input_string.upper()
  	for name in business_names:
    	if name in temp:
    		return name
  	return None

# input receipt image and outputs a dictionary containing info; name, date, and total price
def get_receipt_info(image):
	# image is a string ex:"starbucks-receipt.jpg
	img = cv2.imread(image)
	stringText = pytesseract.image_to_string(img)

	#cleaning text by removing whitespace and new lines
	l = stringText.split("\n")
	res = [ele for ele in l if ele.strip()] 
	res = ' '.join(res)

	receipt_info = {}
	receipt_info["Name"] = find_name(res)
	receipt_info["Date"] = find_date(res)
	receipt_info["Total"] = find_total(res)
	return receipt_info



endpoint = 'database-112.crz5qomac1oy.us-east-2.rds.amazonaws.com'
username = 'testUser'
password = 'confirm1'
database_name = 'Transactions'
connection = pymysql.connect(host = endpoint, user=username, passwd=password, db = database_name, autocommit=True)
def handler(receipt_info):
    cursor = connection.cursor()
    num_rows = cursor.execute('SELECT * from TRANSACTIONS')

    rows = cursor.fetchall()
    
    id_number = int(rows[num_rows-1][0])+ 1
	date = date[6:] + "/" +  date[0:5] #format date yyyy/mm/dd
    date = "\"" + str(receipt_info["Date"]) + "\""
    total = receipt_info["Total"]

    sqlQuery = "INSERT INTO TRANSACTIONS (transaction_id, transaction_date, total, transaction_type) VALUES(" + str(id_number) + ", " + date + ", " + str(total) + ", " + str(1) +")"

    cursor.execute(sqlQuery)


handler(get_receipt_info("receipt.png"))



