WITH PaymentCounts AS (
    SELECT payment_type, COUNT(*) AS cnt 
    FROM payments
    GROUP BY payment_type
    HAVING cnt > 100
)
SELECT payment_type, cnt 
FROM PaymentCounts
ORDER BY cnt DESC
LIMIT 2, 100;
