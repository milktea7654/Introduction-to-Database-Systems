SELECT COUNT(DISTINCT customer_unique_id) AS number_of_diff_id 
FROM customers 
WHERE customer_unique_id LIKE 'ff9%';