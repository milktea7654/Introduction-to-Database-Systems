SELECT customer_city AS city, COUNT(*) AS cnt 
FROM customers
JOIN orders ON customers.customer_id = orders.customer_id
WHERE customer_state = 'RJ'
GROUP BY customer_city
ORDER BY cnt DESC
LIMIT 3;