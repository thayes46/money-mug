CREATE DATABASE Transactions;

 USE Transactions;
 
 CREATE TABLE TRANSACTIONS(
	transaction_id INT PRIMARY KEY,
    total DECIMAL(9,2) NOT NULL,
    transaction_type INT 
 )
 
#desc TRANSACTIONS;
 
 
 #INSERT INTO TRANSACTIONS (transaction_id, total, transaction_type) VALUES
 #(1, 9.99, 1)

 
#SELECT * FROM TRANSACTIONS;