SELECT COUNT(*) AS cnt 
FROM orders
JOIN payments ON orders.order_id = payments.order_id
JOIN customers ON orders.customer_id = customers.customer_id
WHERE payment_type = 'debit_card' 
AND customer_state = 'RJ' 
AND YEAR(order_purchase_timestamp) = 2017;
