WITH DeliveryDays AS (
    SELECT 
        o.order_id,
        c.customer_state,
        (o.order_delivered_customer_date - o.order_delivered_carrier_date) AS delivery_days,
        ROW_NUMBER() OVER (PARTITION BY c.customer_state ORDER BY (o.order_delivered_customer_date - o.order_delivered_carrier_date) DESC) AS rn
    FROM orders o
    JOIN customers c ON o.customer_id = c.customer_id
    WHERE 
        o.order_delivered_customer_date IS NOT NULL 
        AND o.order_delivered_carrier_date IS NOT NULL
)
SELECT 
    customer_state AS state, 
    order_id, 
    delivery_days
FROM DeliveryDays
WHERE rn = 1
ORDER BY customer_state DESC;
