SELECT order_id 
FROM orders
WHERE order_id NOT IN (SELECT order_id FROM payments);