SELECT order_id, (order_delivered_customer_date - order_delivered_carrier_date) as delivery_days 
FROM orders
WHERE order_delivered_customer_date IS NOT NULL AND order_delivered_carrier_date IS NOT NULL 
ORDER BY delivery_days DESC
LIMIT 3;