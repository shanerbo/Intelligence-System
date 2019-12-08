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
                       

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% constraints %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% reference:
% David Mitchell. 
% Notes on Satisfiability-Based Problem Solving Representing Problems: 
% Examples from the 2015 LP/CP Programming Competition. October 28, 2019

% 1. We only pay for the pizzas which is not the free pizzas by applyting coupon:
constraint forall(p in pizzas)(Paid[p] <-> not exists(c in coupons)(UsedFor[c,p]));  
       
% 2. The Used set contains all the coupon we used:
constraint forall(c in coupons)(Used[c] <-> exists(p in pizzas)(UsedFor[c,p]));

% 3. We can only apply coupon c if there is sufficient number of pizza we purchase:
constraint forall(c in coupons)(Used[c] -> sum(p in pizzas )(Justifies[c,p] == true) >= buy[c]);

% 4. For each coupon c, the number of free pizza we get cannot be greater than the number on coupon c:
constraint forall(c in coupons)(Used[c] -> (sum(p in pizzas)(UsedFor[c,p]) <= free[c]));

% 5. The maximum price of free pizzas we claim cannot be greater than the minimum price of purchased pizzas:
constraint forall(c in coupons)(forall (p1, p2 in pizzas) (( p1 != p2 /\ UsedFor[c,p1] /\ Justifies[c,p2]) -> (price[p1] <= price[p2])));

% 6. For each pizza p, if we use p to justi- fy/claim couponc, p has to be paid, which mean p is in Paid:
constraint forall (p in pizzas)(forall(c in coupons)(Justifies[c,p] -> Paid[p]));

% 7. Set upper bound of minimum cost and make sure it will not be greater than the upper bound:
var int: minCost = sum(p in pizzas where Paid[p] == true)(price[p]);
% minCost is the least cost to obtain all pizzas.
var int: maxCost = sum(price);
% if p is not used it will be false, if p is used it will be true so that we could multiply them and sum them up

constraint minCost <= maxCost;

% 8. We cannot justify/claim coupon c which is not in the coupon set:
% constraint forall(c in coupons)(forall (p in pizzas)(Justifies[c, p] -> (c in coupons /\ p in pizzas)));
% constraint forall(c in coupons)(forall (p in pizzas)(UsedFor[c,p] -> (c in coupons /\ p in pizzas)));

% 9. We cannot apply more than one coupon c to the pizza p which is justified for free pizza (this one is missing in the lecture notes):
constraint forall(p in pizzas)((sum(c in coupons)(Justifies[c,p])<=1));

array[coupons] of var string: res;
solve minimize minCost;



output["Coupons: \n"];
output[
        "  \(i). Buy \(buy[i]) get \(free[i]) free \n" | 
             i in coupons
      ];

output [
        "Pizzas Paid :\n  \(Paid)\nCoupons Used:\n  \(Used)\nMinimum cost:\n  \(minCost)\nMaximum cost:\n  \(maxCost)\n" 
        ];
output [if fix(UsedFor[i, j]) then "Applied coupon c\(i) on pizza p\(j)\n"
             else ""
             endif  | 
             i in coupons, j in pizzas] ;
