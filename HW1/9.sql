WITH SellerSales AS (
    SELECT seller_id, COUNT(*) AS cnt 
    FROM order_items
    WHERE price > 150
    GROUP BY seller_id
    HAVING cnt > 100
)
SELECT seller_id, cnt 
FROM SellerSales
ORDER BY cnt DESC
LIMIT 3;
