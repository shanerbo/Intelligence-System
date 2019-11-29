int: n;                                           
% The number n of pizzas;
set of int: pizzas = 1..n; 
                      
% Ordering of pizza
int: m;               
                            
% The number m of coupons;
set of int: coupons = 1..m;   

% Unary function price : [n] → N, giving the price for each of the n pizzas;                    
array[pizzas] of int: price;  
                
% Unary function buy : [m] → N, giving the number of paid pizzas required to justify using each of the m coupons;                        
array[coupons] of int: buy; 
 
% Unary function free : [m] → N, giving the number of free pizzas that can be ob- tained by using each of the m coupons;            
array[coupons] of int: free;  

% Binary relation symbol Justifies, where Justifies(c, p) holds if pizza p is one of the pizzas we will pay for to justify using coupon c;                    
array[coupons, pizzas] of var bool: Justifies;  
   
% Binary relation symbol UsedFor, where UsedFor(c, p) holds if p is one of the pizzas we get free by using coupon c.
array[coupons, pizzas] of var bool: UsedFor;   
    
% Unary relation symbol Paid, for the set of pizzas will be paid for;              
array[pizzas] of var bool: Paid;         
   
% Unary relation symbol Used, for the set of coupons that will be used;
array[coupons] of var bool: Used;  
                       
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% constraint %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 1. We pay for exactly the pizzas that we don’t get free by using coupons:
constraint forall(p in pizzas)(Paid[p] <-> not exists(c in coupons)(UsedFor[c,p]));  
       
% 2. Used is the set of coupons that we use:
constraint forall(c in coupons)(Used[c] <-> exists(p in pizzas)(UsedFor[c,p]));

% 3. Any coupon that is used must be justified by sufficiently many purchased pizzas:
constraint forall(c in coupons)(Used[c] -> sum(p in pizzas)(Justifies[c,p]) >= buy[c]);

% 4. The number of pizzas any coupon is used for is not more than the number it allows us to get free:
constraint forall(c in coupons)(Used[c] -> (sum(p in pizzas)(UsedFor[c,p]) <= free[c]));

% 5. Each free pizza costs at most as much as the cheapest pizza used to justify use of the relevant coupon:
constraint forall(c in coupons)(forall (p1,p2 in pizzas where p1 != p2) ((UsedFor[c,p1] /\ Justifies[c,p2]) -> (price[p1] <= price[p2])));

% 6. We pay for every pizza used to justify use of a coupon:
constraint forall (p in pizzas)(forall(c in coupons)(Justifies[c,p] -> Paid[p]));

% 7. Set upper bound of cost. The total cost is not too large:
var int: minCost = sum(p in pizzas where Paid[p] == true)(price[p]);
var int: maxCost = sum(price);
% if p is not used it will be false, if p is used it will be true so that we could multiply them and sum them up
constraint minCost <= maxCost;


% 8. We also must require that Justifies(c, p) and UsedFor(c, p) hold only of pairs consist- ing of a coupon and a pizza. (For example, if there are 3 coupons and 5 pizzas, we would not want Justifies(4, 5) to hold, as we might then be getting pizza 5 free by applying the non-existent coupon number 4.)
constraint forall(c in coupons)(forall (p in pizzas)(Justifies[c, p] -> (c in coupons /\ p in pizzas)));
constraint forall(c in coupons)(forall (p in pizzas)(UsedFor[c,p] -> (c in coupons /\ p in pizzas)));

% 9. We cannot apply two coupon to the pizza which is jusified for free pizza
constraint forall(p in pizzas)((sum(c in coupons)(Justifies[c,p])<=1));

solve minimize minCost;

output["Coupons: \n"];
output[
        "  " ++ show(i) ++ ". " ++ "Buy " ++ show(buy[i]) ++ " get " ++ show(free[i]) ++ " free\n" | 
             i in coupons
      ];

output [
        "Pizzas Paid :\n  \(Paid)\nCoupons Used:\n  \(Used)\nMinimum cost:\n  \(minCost)\n" 
        ];
output [show(UsedFor[i, j] ) ++ if UsedFor[i, j] == 1 then "Applied coupon c\(i) on pizza p\(j)" 
             else ""
             endif  | 
             i in coupons, j in pizzas
             ];