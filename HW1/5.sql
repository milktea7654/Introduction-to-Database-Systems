SELECT customer_state AS state, COUNT(*) AS cnt 
FROM orders
JOIN customers ON orders.customer_id = customers.customer_id
WHERE order_status != 'delivered'
GROUP BY customer_state;