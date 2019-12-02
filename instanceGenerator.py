from random import seed
from random import randint
seed(10)
combination = [[15,30], [15, 30], [25, 30]]
pizza = []
buy = []
free = []
for i in combination:
    pizzas = i[0]
    coupon = i[1]
    for _ in range(pizzas):
        value = randint(10, 50)
        pizza.append(value)
    for _ in range(coupon):
        value = randint(10, 20)
        buy.append(value)
    for _ in range(coupon):
        value = randint(5, 10)
        free.append(value)
    print(pizza)
    print(buy)
    print(free)
    print('\n')
    pizza.clear()
    buy.clear()
    free.clear()