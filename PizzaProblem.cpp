#include <algorithm>
#include <climits>
#include <iostream>
#include <stdio.h>
#include <unordered_map>
#include <vector>

using namespace std;
typedef vector<tuple<int, int, int>> Coupons;
typedef vector<int> Pizzas;
typedef unordered_map<int, Coupons> Map;
// define some types, easy to read
/*
 * For each coupon, we use tuple<int, int, int> for coupon c,
 * get<0>(tuple) means how may pizzas we need to buy
 * get<1>(tuple) means how many pizzas we can get for free
 * get<2>(tuple) means if this coupon is used, 1: used, 0: not used
 */
class PizzaProblem {
private:
  void find(Pizzas &pizzas, Coupons &coupons, Map &map, int start, int obtain,
            int cost, int total, int used, int &res) {
    if (obtain >= total) {
      res = min(res, cost);
      return;
    }
    int left = total - obtain;
    int canUseCoupon = false;
    // if there is a such valid coupon we can use, if there is no coupon we
    // could use, we should buy all left pizzas
    for (int i = left; i >= 0; --i) {
      if (map.count(i) > 0) {
        for (int j = 0; j < map[i].size(); ++j) {
          if (get<2>(map[i][j]) == 0) {
            canUseCoupon = true;
            break;
          }
        }
        if (canUseCoupon) {
          break;
        }
      }
    }
    // If we used all coupons, we need to buy the rest of pizzas
    if (used == coupons.size() || !canUseCoupon) {
      for (int i = start; i < pizzas.size(); ++i) {
        cost += pizzas[i];
        // cout<<pizzas[i]<<endl;
      }
      res = min(res, cost);
      return;
    }
    // We still have coupon we can use
    if (canUseCoupon) {
      int purchased = 0;
      int sum = 0;
      for (int i = start; i < total; ++i) {
        sum += pizzas[i];
        if (map.count(++purchased) > 0) {
          // there is a coupon we could claim
          int size = map[purchased].size();
          for (int p = size - 1; p >= 0; --p) {
            if (get<2>(map[purchased][p]) == 0) {
              // get<2>(map[purchased][p]) == 0 means this coupon is not used
              // yet, we could try to use it;
              int freePizzas = get<1>(map[purchased][p]);
              // how many pizza we can get for free from this coupon;
              get<2>(map[purchased][p]) = 1;
              // set this coupon to 1, which means this coupon is used;
              // cout<<"purchased: "<< purchased << " get "<< freePizzas<< endl;
              find(pizzas, coupons, map, i + freePizzas + 1,
                   obtain + purchased + freePizzas, sum + cost, total, used + 1,
                   res);
              get<2>(map[purchased][p]) = 0;
              // set this coupon back to 0, which means we no longer use it
            }
          }
        }
      }
    }
    return;
  };

public:
  int miniCost(Pizzas &pizzas, Coupons coupons) {
    Map map;
    int res = INT_MAX;
    auto cmp = [](tuple<int, int, int> a, tuple<int, int, int> b) {
      return get<1>(a) < get<1>(b);
    };
    // customized compare function
    sort(coupons.begin(), coupons.end(), cmp);
    for (auto const &c : coupons) {
      map[get<0>(c)].push_back(c);
    }
    // add coupon into map, use 'buy' as key, 'free' as value, and the value is
    // vector<tuple>
    sort(pizzas.rbegin(), pizzas.rend());
    // sort pizzas array descending order
    find(pizzas, coupons, map, 0, 0, 0, pizzas.size(), 0, res);
    return res == INT_MAX ? -1 : res;
    // if res is equal to INT_MAX mean there is no result, we return -1
  };
};

Coupons convert(vector<int> buy, vector<int> free) {
  // combine buy and free vectors into one vector<tuple<int, int, int>>
  Coupons coupons;
  for (int i = 0; i < buy.size(); ++i) {
    coupons.push_back(make_tuple(buy[i], free[i], 0));
  }
  return coupons;
}

// test
int main() {
  auto pizzaProblem = new PizzaProblem();
  int res;
  Pizzas pizzas = {};
  Coupons coupons = {};
  vector<int> buy = {};
  vector<int> free = {};
  //     sanity test
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 1 Correct answer: 0, our answer: " << res << endl;

  //     dummy test
  pizzas = {11, 14, 22, 5, 3, 19};
  buy = {1, 2, 2, 3};
  free = {1, 1, 2, 2};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 2 Correct answer: 46, our answer: " << res << endl;

  //     Web test 1
  pizzas = {70, 10, 60, 60, 30, 100, 60, 40, 60, 20};
  buy = {1, 2, 1, 1};
  free = {1, 1, 1, 0};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 3 Correct answer: 340, our answer: " << res << endl;

  //     Web test 2
  pizzas = {10, 15, 20, 15};
  buy = {1, 2, 2, 8, 3, 1, 4};
  free = {1, 1, 2, 9, 1, 0, 1};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 4 Correct answer: 35, our answer: " << res << endl;

  // test 10
  pizzas = {46, 45, 39, 41, 24, 30, 20, 49, 27, 40, 29, 29, 42, 45, 43};
  buy = {11, 14, 11, 9,  9,  9,  14, 12, 12, 13, 10, 7,  15, 7,  12,
         12, 10, 8,  11, 14, 13, 9,  13, 14, 10, 15, 11, 15, 14, 14};
  free = {5, 6, 8, 10, 8, 8, 6, 8, 9, 9, 7, 9, 8, 8, 10,
          6, 9, 8, 5,  8, 5, 5, 8, 9, 5, 5, 7, 5, 6, 5};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 10 Correct answer: 311, our answer: " << res << endl;

  // test 12
  pizzas = {48, 39, 41, 18, 39, 37, 42, 38, 31, 26, 39, 37, 31,
            43, 43, 19, 25, 30, 49, 12, 22, 47, 46, 50, 40};
  buy = {15, 20, 18, 18, 12, 13, 15, 19, 10, 20, 13, 20, 19, 19, 20,
         17, 10, 11, 14, 16, 13, 14, 11, 10, 19, 18, 11, 14, 12, 20};
  free = {8, 6, 7,  9,  8, 6,  7, 6, 5,  9,  6, 8, 6, 5, 10,
          7, 8, 10, 10, 9, 10, 8, 8, 10, 10, 9, 6, 7, 9, 7};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 12 Correct answer: 545, our answer: " << res << endl;

  // test 13
  pizzas = {46, 12, 37, 40, 46, 10, 23, 39, 41, 27, 20, 12,
            43, 41, 30, 14, 25, 33, 12, 36, 18, 48, 32, 34,
            36, 28, 26, 39, 21, 29, 33, 18, 39, 25, 38};
  buy = {70, 63, 52, 69, 51, 58, 55, 57, 60, 68, 62, 58, 61,
         68, 65, 64, 66, 53, 69, 61, 67, 56, 58, 64, 58};
  free = {5, 5, 8, 7, 9, 10, 5, 9, 5, 6, 8, 9, 7,
          9, 6, 5, 5, 8, 6,  6, 7, 8, 8, 8, 6};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 13 Correct answer: 1051, our answer: " << res << endl;

  // test 14
  pizzas = {27, 19, 49, 43, 21, 17, 27, 39, 29, 20, 21, 21, 40, 32,
            30, 37, 24, 10, 44, 12, 31, 30, 25, 15, 26, 38, 35, 47,
            20, 34, 41, 25, 43, 27, 43, 40, 48, 41, 14, 20};
  buy = {66, 65, 63, 55, 64, 68, 69, 62, 68, 63, 66, 56, 68, 65, 54,
         64, 52, 51, 65, 68, 53, 52, 62, 53, 55, 54, 70, 65, 66, 55};
  free = {8,  8, 9, 8, 7, 10, 7, 10, 8, 8, 7,  10, 9, 9, 6,
          10, 6, 7, 9, 5, 10, 6, 10, 9, 9, 10, 8,  5, 5, 7};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 14 Correct answer: 1205, our answer: " << res << endl;

  // test 15
  pizzas = {37, 25, 28, 15, 12, 48, 45, 16, 28, 20, 40, 19, 33, 48, 35,
            22, 32, 21, 15, 47, 22, 37, 23, 16, 29, 39, 43, 37, 38, 45,
            23, 27, 48, 33, 17, 50, 50, 10, 23, 37, 49, 36, 34, 22, 14};
  buy = {64, 53, 52, 62, 59, 62, 65, 64, 57, 60, 65, 60, 67, 66, 67, 59, 63, 65,
         61, 62, 59, 66, 65, 66, 69, 66, 53, 68, 62, 63, 56, 70, 64, 69, 53};
  free = {9, 10, 7, 5, 9, 7, 10, 7, 5, 10, 8, 9, 7, 7, 9, 6, 6, 6,
          7, 5,  9, 5, 6, 9, 10, 5, 8, 6,  9, 9, 8, 9, 5, 8, 6};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 15 Correct answer: 1388, our answer: " << res << endl;

  // test 16
  pizzas = {31, 46, 22, 43, 46, 30, 13, 47, 13, 12, 46, 33, 35, 30, 48, 50, 20,
            20, 17, 48, 32, 20, 11, 44, 42, 32, 15, 35, 38, 46, 32, 13, 19, 38,
            40, 49, 38, 38, 48, 21, 32, 37, 31, 45, 42, 39, 34, 22, 16, 37};
  buy = {55, 52, 56, 57, 69, 64, 62, 68, 63, 52, 70, 52, 56, 60,
         58, 52, 69, 56, 54, 69, 62, 56, 60, 52, 63, 70, 64, 53,
         62, 57, 66, 56, 56, 63, 65, 61, 52, 56, 56, 69};
  free = {10, 8, 6, 9, 10, 10, 8, 5, 8, 5, 6,  5, 8,  10, 10, 7, 10, 7, 9,  10,
          5,  7, 9, 9, 8,  9,  6, 9, 7, 5, 10, 6, 10, 8,  10, 6, 5,  9, 10, 8};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 16 Correct answer: 1636, our answer: " << res << endl;

  // test 17
  pizzas = {46, 12, 37, 40, 46, 10, 23, 39, 41, 27, 20, 12,
            43, 41, 30, 14, 25, 33, 12, 36, 18, 48, 32, 34,
            36, 28, 26, 39, 21, 29, 33, 18, 39, 25, 38};
  buy = {39, 32, 21, 38, 20, 27, 24, 26, 50, 49, 29, 37, 31,
         44, 48, 27, 30, 41, 37, 34, 33, 35, 22, 40, 38};
  free = {7, 9, 6, 6, 8, 6, 5, 5, 8, 7, 9, 10, 5,
          9, 5, 6, 8, 9, 7, 9, 6, 5, 5, 8, 6};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 17 Correct answer: 856, our answer: " << res << endl;

  // test 18
  pizzas = {22, 32, 37, 36, 38, 25, 27, 19, 49, 43, 21, 17, 27, 39,
            29, 20, 21, 21, 40, 32, 30, 37, 24, 10, 44, 12, 31, 30,
            25, 15, 26, 38, 35, 47, 20, 34, 41, 25, 43, 27};
  buy = {20, 33, 23, 26, 27, 34, 28, 30, 46, 22, 29, 40, 29, 23, 36,
         45, 21, 20, 32, 44, 37, 45, 33, 35, 31, 42, 44, 27, 38, 43};
  free = {5, 8, 10, 6, 5, 8, 9, 7, 9, 7, 6, 10, 7, 6, 10,
          7, 5, 5,  6, 7, 6, 5, 9, 6, 5, 5, 5,  8, 6, 8};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 18 Correct answer: 952, our answer: " << res << endl;

  // test 19
  pizzas = {28, 49, 10, 32, 28, 19, 40, 45, 27, 18, 15, 45, 28, 17, 14,
            36, 38, 19, 45, 34, 47, 20, 27, 48, 48, 41, 32, 16, 13, 27,
            43, 18, 21, 50, 15, 20, 24, 10, 22, 21, 40, 39, 47, 45, 44};
  buy = {32, 22, 25, 37, 35, 38, 50, 28, 37, 22, 50, 40, 20, 38, 38, 48, 33, 37,
         29, 36, 36, 37, 40, 49, 45, 22, 48, 40, 38, 37, 50, 29, 22, 43, 27};
  free = {10, 6, 5,  5, 10, 8, 8, 5, 8, 10, 10, 5, 10, 9,  5, 7, 6, 8,
          6,  7, 10, 9, 9,  8, 6, 5, 9, 9,  5,  9, 5,  10, 8, 8, 9};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 19 Correct answer: N/A, our answer: " << res << endl;

  // test 20
  pizzas = {14, 15, 22, 12, 48, 21, 15, 14, 11, 32, 49, 40, 45, 44, 40, 11, 14,
            31, 15, 30, 10, 39, 38, 47, 19, 34, 40, 18, 16, 32, 24, 38, 18, 22,
            24, 16, 17, 13, 17, 14, 15, 33, 16, 11, 39, 50, 43, 22, 33, 50};
  buy = {22, 27, 20, 28, 44, 36, 41, 26, 20, 34, 28, 50, 39, 20,
         50, 23, 36, 45, 33, 21, 48, 28, 35, 47, 30, 39, 47, 27,
         40, 21, 41, 28, 24, 29, 21, 45, 25, 31, 45, 48};
  free = {8, 5, 7, 5, 5, 7, 8, 6, 8, 9, 7, 8, 9, 7, 8, 8, 6, 5, 7, 7,
          8, 8, 9, 9, 8, 8, 8, 6, 7, 8, 5, 7, 5, 6, 7, 9, 5, 6, 6, 10};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 20 Correct answer: N/A, our answer: " << res << endl;

  // test 21
  pizzas = {18, 46, 14, 26, 17, 41, 38, 40, 34, 23, 16, 41, 11, 34, 37, 48, 10,
            38, 27, 24, 47, 16, 30, 11, 11, 11, 44, 10, 34, 23, 37, 11, 43, 24,
            38, 41, 45, 24, 32, 24, 24, 39, 28, 11, 36, 45, 16, 21, 50, 28, 17,
            31, 42, 37, 42, 22, 29, 28, 47, 41, 42, 35, 47, 12, 40, 25, 35, 36,
            21, 33, 45, 33, 15, 38, 42, 16, 20, 43, 35, 33, 41, 11, 40, 12, 29,
            49, 47, 47, 35, 20, 20, 42, 24, 10, 22, 44, 45, 24, 35, 42};
  buy = {48, 46, 52, 49, 51, 59, 53, 46, 51, 42, 49, 56, 58, 40, 60, 58, 44,
         49, 50, 57, 58, 51, 52, 54, 42, 57, 42, 44, 49, 51, 60, 48, 49, 40,
         60, 54, 60, 55, 44, 46, 43, 59, 54, 59, 43, 60, 44, 55, 41, 51};
  free = {5,  8, 5,  7,  6, 7, 5,  9, 6, 7, 7, 5, 6, 6, 7,  9, 6,
          10, 7, 10, 10, 7, 8, 10, 7, 8, 8, 5, 5, 7, 8, 7,  8, 6,
          7,  5, 7,  10, 9, 6, 9,  8, 5, 6, 5, 8, 6, 5, 10, 6};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 21 Correct answer: N/A, our answer: " << res << endl;

  // test 22
  pizzas = {46, 29, 12, 17, 11, 50, 10, 26, 42, 30, 21, 16, 46, 34, 10, 29, 11,
            46, 24, 11, 31, 47, 47, 27, 42, 43, 20, 17, 15, 30, 11, 36, 19, 13,
            33, 39, 36, 26, 31, 34, 43, 41, 47, 30, 45, 20, 14, 29, 41, 44, 42,
            47, 37, 14, 34, 23, 40, 15, 24, 19, 39, 48, 45, 46, 13, 46, 31, 43,
            12, 47, 48, 12, 43, 19, 34, 37, 40, 20, 32, 37, 39, 46, 24, 42, 31,
            43, 22, 32, 23, 11, 14, 24, 21, 11, 34, 20, 31, 35, 19, 14};
  buy = {14, 13, 20, 16, 14, 15, 19, 16, 13, 15, 11, 14, 18, 19, 10, 20, 19,
         12, 14, 15, 18, 19, 15, 16, 17, 11, 18, 11, 12, 14, 15, 20, 14, 14,
         10, 20, 17, 20, 17, 12, 13, 11, 19, 17, 19, 11, 20, 12, 17, 10};
  free = {15, 10, 12, 15, 14, 20, 20, 20, 19, 12, 12, 17, 14, 14, 18, 19, 14,
          16, 12, 20, 17, 20, 11, 15, 17, 12, 12, 20, 13, 16, 10, 14, 10, 11,
          15, 11, 12, 19, 11, 13, 17, 19, 10, 10, 16, 19, 14, 20, 17, 11};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 22 Correct answer: N/A, our answer: " << res << endl;
  return 0;
}