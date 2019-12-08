#include <algorithm>
#include <climits>
#include <cstdio>
#include <ctime>
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
  std::clock_t start;
  double duration;
  start = std::clock();
  //     sanity test
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 1 Correct answer: 0, our answer: " << res
       << " Time: " << duration << endl;
  ;

  //     dummy test
  start = std::clock();
  pizzas = {11, 14, 22, 5, 3, 19};
  buy = {1, 2, 2, 3};
  free = {1, 1, 2, 2};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  cout << "Test 2 Correct answer: 46, our answer: " << res
       << " Time: " << duration << endl;

  //     Web test 1
  start = std::clock();
  pizzas = {70, 10, 60, 60, 30, 100, 60, 40, 60, 20};
  buy = {1, 2, 1, 1};
  free = {1, 1, 1, 0};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 3 Correct answer: 340, our answer: " << res
       << " Time: " << duration << endl;

  //     Web test 2
  start = std::clock();
  pizzas = {10, 15, 20, 15};
  buy = {1, 2, 2, 8, 3, 1, 4};
  free = {1, 1, 2, 9, 1, 0, 1};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 4 Correct answer: 35, our answer: " << res
       << " Time: " << duration << endl;

  // test 10
  start = std::clock();
  pizzas = {46, 45, 39, 41, 24, 30, 20, 49, 27, 40, 29, 29, 42, 45, 43};
  buy = {11, 14, 11, 9,  9,  9,  14, 12, 12, 13, 10, 7,  15, 7,  12,
         12, 10, 8,  11, 14, 13, 9,  13, 14, 10, 15, 11, 15, 14, 14};
  free = {5, 6, 8, 10, 8, 8, 6, 8, 9, 9, 7, 9, 8, 8, 10,
          6, 9, 8, 5,  8, 5, 5, 8, 9, 5, 5, 7, 5, 6, 5};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 10 Correct answer: 311, our answer: " << res
       << " Time: " << duration << endl;

  // test 12
  start = std::clock();
  pizzas = {48, 39, 41, 18, 39, 37, 42, 38, 31, 26, 39, 37, 31,
            43, 43, 19, 25, 30, 49, 12, 22, 47, 46, 50, 40};
  buy = {15, 20, 18, 18, 12, 13, 15, 19, 10, 20, 13, 20, 19, 19, 20,
         17, 10, 11, 14, 16, 13, 14, 11, 10, 19, 18, 11, 14, 12, 20};
  free = {8, 6, 7,  9,  8, 6,  7, 6, 5,  9,  6, 8, 6, 5, 10,
          7, 8, 10, 10, 9, 10, 8, 8, 10, 10, 9, 6, 7, 9, 7};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 12 Correct answer: 545, our answer: " << res
       << " Time: " << duration << endl;

  // test 13
  start = std::clock();
  pizzas = {46, 12, 37, 40, 46, 10, 23, 39, 41, 27, 20, 12,
            43, 41, 30, 14, 25, 33, 12, 36, 18, 48, 32, 34,
            36, 28, 26, 39, 21, 29, 33, 18, 39, 25, 38};
  buy = {70, 63, 52, 69, 51, 58, 55, 57, 60, 68, 62, 58, 61,
         68, 65, 64, 66, 53, 69, 61, 67, 56, 58, 64, 58};
  free = {5, 5, 8, 7, 9, 10, 5, 9, 5, 6, 8, 9, 7,
          9, 6, 5, 5, 8, 6,  6, 7, 8, 8, 8, 6};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 13 Correct answer: 1051, our answer: " << res
       << " Time: " << duration << endl;

  // test 14
  start = std::clock();
  pizzas = {27, 19, 49, 43, 21, 17, 27, 39, 29, 20, 21, 21, 40, 32,
            30, 37, 24, 10, 44, 12, 31, 30, 25, 15, 26, 38, 35, 47,
            20, 34, 41, 25, 43, 27, 43, 40, 48, 41, 14, 20};
  buy = {66, 65, 63, 55, 64, 68, 69, 62, 68, 63, 66, 56, 68, 65, 54,
         64, 52, 51, 65, 68, 53, 52, 62, 53, 55, 54, 70, 65, 66, 55};
  free = {8,  8, 9, 8, 7, 10, 7, 10, 8, 8, 7,  10, 9, 9, 6,
          10, 6, 7, 9, 5, 10, 6, 10, 9, 9, 10, 8,  5, 5, 7};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 14 Correct answer: 1205, our answer: " << res
       << " Time: " << duration << endl;

  // test 15
  start = std::clock();
  pizzas = {37, 25, 28, 15, 12, 48, 45, 16, 28, 20, 40, 19, 33, 48, 35,
            22, 32, 21, 15, 47, 22, 37, 23, 16, 29, 39, 43, 37, 38, 45,
            23, 27, 48, 33, 17, 50, 50, 10, 23, 37, 49, 36, 34, 22, 14};
  buy = {64, 53, 52, 62, 59, 62, 65, 64, 57, 60, 65, 60, 67, 66, 67, 59, 63, 65,
         61, 62, 59, 66, 65, 66, 69, 66, 53, 68, 62, 63, 56, 70, 64, 69, 53};
  free = {9, 10, 7, 5, 9, 7, 10, 7, 5, 10, 8, 9, 7, 7, 9, 6, 6, 6,
          7, 5,  9, 5, 6, 9, 10, 5, 8, 6,  9, 9, 8, 9, 5, 8, 6};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 15 Correct answer: 1388, our answer: " << res
       << " Time: " << duration << endl;

  // test 16
  start = std::clock();
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
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 16 Correct answer: 1636, our answer: " << res
       << " Time: " << duration << endl;

  // test 17
  start = std::clock();
  pizzas = {46, 12, 37, 40, 46, 10, 23, 39, 41, 27, 20, 12,
            43, 41, 30, 14, 25, 33, 12, 36, 18, 48, 32, 34,
            36, 28, 26, 39, 21, 29, 33, 18, 39, 25, 38};
  buy = {39, 32, 21, 38, 20, 27, 24, 26, 50, 49, 29, 37, 31,
         44, 48, 27, 30, 41, 37, 34, 33, 35, 22, 40, 38};
  free = {7, 9, 6, 6, 8, 6, 5, 5, 8, 7, 9, 10, 5,
          9, 5, 6, 8, 9, 7, 9, 6, 5, 5, 8, 6};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 17 Correct answer: 856, our answer: " << res
       << " Time: " << duration << endl;
  // test 18
  start = std::clock();
  pizzas = {22, 32, 37, 36, 38, 25, 27, 19, 49, 43, 21, 17, 27, 39,
            29, 20, 21, 21, 40, 32, 30, 37, 24, 10, 44, 12, 31, 30,
            25, 15, 26, 38, 35, 47, 20, 34, 41, 25, 43, 27};
  buy = {20, 33, 23, 26, 27, 34, 28, 30, 46, 22, 29, 40, 29, 23, 36,
         45, 21, 20, 32, 44, 37, 45, 33, 35, 31, 42, 44, 27, 38, 43};
  free = {5, 8, 10, 6, 5, 8, 9, 7, 9, 7, 6, 10, 7, 6, 10,
          7, 5, 5,  6, 7, 6, 5, 9, 6, 5, 5, 5,  8, 6, 8};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 18 Correct answer: 952, our answer: " << res
       << " Time: " << duration << endl;
  // test 19
  start = std::clock();
  pizzas = {28, 49, 10, 32, 28, 19, 40, 45, 27, 18, 15, 45, 28, 17, 14,
            36, 38, 19, 45, 34, 47, 20, 27, 48, 48, 41, 32, 16, 13, 27,
            43, 18, 21, 50, 15, 20, 24, 10, 22, 21, 40, 39, 47, 45, 44};
  buy = {32, 22, 25, 37, 35, 38, 50, 28, 37, 22, 50, 40, 20, 38, 38, 48, 33, 37,
         29, 36, 36, 37, 40, 49, 45, 22, 48, 40, 38, 37, 50, 29, 22, 43, 27};
  free = {10, 6, 5,  5, 10, 8, 8, 5, 8, 10, 10, 5, 10, 9,  5, 7, 6, 8,
          6,  7, 10, 9, 9,  8, 6, 5, 9, 9,  5,  9, 5,  10, 8, 8, 9};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 19 Correct answer: N/A, our answer: " << res
       << " Time: " << duration << endl;
  // test 20
  start = std::clock();
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
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 20 Correct answer: N/A, our answer: " << res
       << " Time: " << duration << endl;
  // test 21
  start = std::clock();
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
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 21 Correct answer: N/A, our answer: " << res
       << " Time: " << duration << endl;
  // test 22
  start = std::clock();
  pizzas = {46, 29, 12, 17, 11, 50, 10, 26, 42, 30, 21, 16, 46, 34, 10, 29, 11,
            46, 24, 11, 31, 47, 47, 27, 42, 43, 20, 17, 15, 30, 11, 36, 19, 13,
            33, 39, 36, 26, 31, 34, 43, 41, 47, 30, 45, 20, 14, 29, 41, 44, 42,
            47, 37, 14, 34, 23, 40, 15, 24, 19, 39, 48, 45, 46, 13, 46, 31, 43,
            12, 47, 48, 12, 43, 19, 34, 37, 40, 20, 32, 37, 39, 46, 24, 42, 31,
            43, 22, 32, 23, 11, 14, 24, 21, 11, 34, 20, 31, 35, 19, 14, 27, 23,
            34, 28, 33, 49, 37, 23, 33, 14, 28, 42, 46, 10, 50, 47, 18, 28, 31,
            45, 46, 33, 35, 38, 15, 45, 14, 19, 29, 32, 26, 29, 10, 50, 39, 50,
            41, 19, 23, 17, 48, 39, 49, 16, 19, 41, 12, 32, 12, 20, 33, 27, 50,
            46, 18, 18, 39, 29, 29, 45, 47, 27, 37, 18, 40, 17, 33, 38, 20, 18,
            22, 34, 13, 27, 13, 15, 30, 17, 19, 49, 14, 24, 40, 48, 11, 10, 36,
            46, 26, 40, 14, 43, 11, 25, 37, 44, 23, 25, 23, 32};
  buy = {82, 98,  98,  94, 88, 81, 85,  82,  90, 96, 100, 80, 82, 100, 92,
         99, 100, 90,  83, 86, 88, 94,  92,  99, 85, 82,  84, 86, 83,  99,
         94, 83,  82,  96, 92, 80, 100, 86,  85, 99, 81,  86, 93, 90,  100,
         81, 86,  94,  96, 95, 98, 88,  100, 87, 91, 86,  87, 86, 91,  82,
         99, 94,  91,  90, 96, 88, 86,  94,  80, 92, 89,  82, 87, 93,  80,
         98, 100, 100, 83, 80, 93, 91,  91,  82, 83, 94,  88, 86, 99,  91,
         86, 88,  85,  90, 88, 82, 90,  97,  94, 87};
  free = {45, 47, 48, 43, 49, 51, 59, 46, 59, 53, 50, 51, 48, 56, 57, 40, 41,
          54, 58, 44, 42, 56, 56, 47, 59, 49, 60, 54, 46, 53, 52, 44, 45, 45,
          59, 58, 58, 57, 46, 52, 49, 47, 44, 45, 56, 49, 43, 41, 50, 50, 40,
          53, 41, 45, 60, 60, 51, 42, 50, 45, 50, 40, 53, 41, 42, 52, 47, 52,
          40, 46, 44, 55, 46, 60, 54, 57, 47, 49, 44, 45, 55, 48, 47, 57, 44,
          53, 60, 59, 55, 50, 60, 56, 48, 58, 41, 42, 46, 41, 60, 47};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 22 Correct answer: N/A, our answer: " << res
       << " Time: " << duration << endl;

  // test 23
  start = std::clock();
  pizzas = {46, 29, 12, 17, 11, 50, 10, 26, 42, 30, 21, 16, 46, 34, 10, 29, 11,
            46, 24, 11, 31, 47, 47, 27, 42, 43, 20, 17, 15, 30, 11, 36, 19, 13,
            33, 39, 36, 26, 31, 34, 43, 41, 47, 30, 45, 20, 14, 29, 41, 44, 42,
            47, 37, 14, 34, 23, 40, 15, 24, 19, 39, 48, 45, 46, 13, 46, 31, 43,
            12, 47, 48, 12, 43, 19, 34, 37, 40, 20, 32, 37, 39, 46, 24, 42, 31,
            43, 22, 32, 23, 11, 14, 24, 21, 11, 34, 20, 31, 35, 19, 14, 27, 23,
            34, 28, 33, 49, 37, 23, 33, 14, 28, 42, 46, 10, 50, 47, 18, 28, 31,
            45, 46, 33, 35, 38, 15, 45, 14, 19, 29, 32, 26, 29, 10, 50, 39, 50,
            41, 19, 23, 17, 48, 39, 49, 16, 19, 41, 12, 32, 12, 20, 33, 27, 50,
            46, 18, 18, 39, 29, 29, 45, 47, 27, 37, 18, 40, 17, 33, 38, 20, 18,
            22, 34, 13, 27, 13, 15, 30, 17, 19, 49, 14, 24, 40, 48, 11, 10, 36,
            46, 26, 40, 14, 43, 11, 25, 37, 44, 23, 25, 23, 32};
  buy = {62, 78, 78, 74, 68, 61, 65, 62, 70, 76, 80, 60, 62, 80, 72, 79, 80,
         70, 63, 66, 68, 74, 72, 79, 65, 62, 64, 66, 63, 79, 74, 63, 62, 76,
         72, 60, 80, 66, 65, 79, 61, 66, 73, 70, 80, 61, 66, 74, 76, 75, 78,
         68, 80, 67, 71, 66, 67, 66, 71, 62, 79, 74, 71, 70, 76, 68, 66, 74,
         60, 72, 69, 62, 67, 73, 60, 78, 80, 80, 63, 60, 73, 71, 71, 62, 63,
         74, 68, 66, 79, 71, 66, 68, 65, 70, 68, 62, 70, 77, 74, 67};
  free = {45, 47, 48, 43, 49, 51, 59, 46, 59, 53, 50, 51, 48, 56, 57, 40, 41,
          54, 58, 44, 42, 56, 56, 47, 59, 49, 60, 54, 46, 53, 52, 44, 45, 45,
          59, 58, 58, 57, 46, 52, 49, 47, 44, 45, 56, 49, 43, 41, 50, 50, 40,
          53, 41, 45, 60, 60, 51, 42, 50, 45, 50, 40, 53, 41, 42, 52, 47, 52,
          40, 46, 44, 55, 46, 60, 54, 57, 47, 49, 44, 45, 55, 48, 47, 57, 44,
          53, 60, 59, 55, 50, 60, 56, 48, 58, 41, 42, 46, 41, 60, 47};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 23 Correct answer: N/A, our answer: " << res
       << " Time: " << duration << endl;

  // test 24
  start = std::clock();
  pizzas = {46, 29, 12, 17, 11, 50, 10, 26, 42, 30, 21, 16, 46, 34, 10, 29, 11,
            46, 24, 11, 31, 47, 47, 27, 42, 43, 20, 17, 15, 30, 11, 36, 19, 13,
            33, 39, 36, 26, 31, 34, 43, 41, 47, 30, 45, 20, 14, 29, 41, 44, 42,
            47, 37, 14, 34, 23, 40, 15, 24, 19, 39, 48, 45, 46, 13, 46, 31, 43,
            12, 47, 48, 12, 43, 19, 34, 37, 40, 20, 32, 37, 39, 46, 24, 42, 31,
            43, 22, 32, 23, 11, 14, 24, 21, 11, 34, 20, 31, 35, 19, 14, 27, 23,
            34, 28, 33, 49, 37, 23, 33, 14, 28, 42, 46, 10, 50, 47, 18, 28, 31,
            45, 46, 33, 35, 38, 15, 45, 14, 19, 29, 32, 26, 29, 10, 50, 39, 50,
            41, 19, 23, 17, 48, 39, 49, 16, 19, 41, 12, 32, 12, 20, 33, 27, 50,
            46, 18, 18, 39, 29, 29, 45, 47, 27, 37, 18, 40, 17, 33, 38, 20, 18,
            22, 34, 13, 27, 13, 15, 30, 17, 19, 49, 14, 24, 40, 48, 11, 10, 36,
            46, 26, 40, 14, 43, 11, 25, 37, 44, 23, 25, 23, 32};
  buy = {42, 58, 58, 54, 48, 41, 45, 42, 50, 56, 60, 40, 42, 60, 52, 59, 60,
         50, 43, 46, 48, 54, 52, 59, 45, 42, 44, 46, 43, 59, 54, 43, 42, 56,
         52, 40, 60, 46, 45, 59, 41, 46, 53, 50, 60, 41, 46, 54, 56, 55, 58,
         48, 60, 47, 51, 46, 47, 46, 51, 42, 59, 54, 51, 50, 56, 48, 46, 54,
         40, 52, 49, 42, 47, 53, 40, 58, 60, 60, 43, 40, 53, 51, 51, 42, 43,
         54, 48, 46, 59, 51, 46, 48, 45, 50, 48, 42, 50, 57, 54, 47};
  free = {25, 27, 28, 23, 29, 31, 39, 26, 39, 33, 30, 31, 28, 36, 37, 20, 21,
          34, 38, 24, 22, 36, 36, 27, 39, 29, 40, 34, 26, 33, 32, 24, 25, 25,
          39, 38, 38, 37, 26, 32, 29, 27, 24, 25, 36, 29, 23, 21, 30, 30, 20,
          33, 21, 25, 40, 40, 31, 22, 30, 25, 30, 20, 33, 21, 22, 32, 27, 32,
          20, 26, 24, 35, 26, 40, 34, 37, 27, 29, 24, 25, 35, 28, 27, 37, 24,
          33, 40, 39, 35, 30, 40, 36, 28, 38, 21, 22, 26, 21, 40, 27};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 24 Correct answer: N/A, our answer: " << res
       << " Time: " << duration << endl;

  // test 25
  start = std::clock();
  pizzas = {46, 29, 12, 17, 11, 50, 10, 26, 42, 30, 21, 16, 46, 34, 10, 29, 11,
            46, 24, 11, 31, 47, 47, 27, 42, 43, 20, 17, 15, 30, 11, 36, 19, 13,
            33, 39, 36, 26, 31, 34, 43, 41, 47, 30, 45, 20, 14, 29, 41, 44, 42,
            47, 37, 14, 34, 23, 40, 15, 24, 19, 39, 48, 45, 46, 13, 46, 31, 43,
            12, 47, 48, 12, 43, 19, 34, 37, 40, 20, 32, 37, 39, 46, 24, 42, 31,
            43, 22, 32, 23, 11, 14, 24, 21, 11, 34, 20, 31, 35, 19, 14, 27, 23,
            34, 28, 33, 49, 37, 23, 33, 14, 28, 42, 46, 10, 50, 47, 18, 28, 31,
            45, 46, 33, 35, 38, 15, 45, 14, 19, 29, 32, 26, 29, 10, 50, 39, 50,
            41, 19, 23, 17, 48, 39, 49, 16, 19, 41, 12, 32, 12, 20, 33, 27, 50,
            46, 18, 18, 39, 29, 29, 45, 47, 27, 37, 18, 40, 17, 33, 38, 20, 18,
            22, 34, 13, 27, 13, 15, 30, 17, 19, 49, 14, 24, 40, 48, 11, 10, 36,
            46, 26, 40, 14, 43, 11, 25, 37, 44, 23, 25, 23, 32};
  buy = {22, 38, 38, 34, 28, 21, 25, 22, 30, 36, 40, 20, 22, 40, 32, 39, 40,
         30, 23, 26, 28, 34, 32, 39, 25, 22, 24, 26, 23, 39, 34, 23, 22, 36,
         32, 20, 40, 26, 25, 39, 21, 26, 33, 30, 40, 21, 26, 34, 36, 35, 38,
         28, 40, 27, 31, 26, 27, 26, 31, 22, 39, 34, 31, 30, 36, 28, 26, 34,
         20, 32, 29, 22, 27, 33, 20, 38, 40, 40, 23, 20, 33, 31, 31, 22, 23,
         34, 28, 26, 39, 31, 26, 28, 25, 30, 28, 22, 30, 37, 34, 27};
  free = {45, 47, 48, 43, 49, 51, 59, 46, 59, 53, 50, 51, 48, 56, 57, 40, 41,
          54, 58, 44, 42, 56, 56, 47, 59, 49, 60, 54, 46, 53, 52, 44, 45, 45,
          59, 58, 58, 57, 46, 52, 49, 47, 44, 45, 56, 49, 43, 41, 50, 50, 40,
          53, 41, 45, 60, 60, 51, 42, 50, 45, 50, 40, 53, 41, 42, 52, 47, 52,
          40, 46, 44, 55, 46, 60, 54, 57, 47, 49, 44, 45, 55, 48, 47, 57, 44,
          53, 60, 59, 55, 50, 60, 56, 48, 58, 41, 42, 46, 41, 60, 47};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 25 Correct answer: N/A, our answer: " << res
       << " Time: " << duration << endl;

  // test 26
  start = std::clock();
  pizzas = {46, 29, 12, 17, 11, 50, 10, 26, 42, 30, 21, 16, 46, 34, 10, 29, 11,
            46, 24, 11, 31, 47, 47, 27, 42, 43, 20, 17, 15, 30, 11, 36, 19, 13,
            33, 39, 36, 26, 31, 34, 43, 41, 47, 30, 45, 20, 14, 29, 41, 44, 42,
            47, 37, 14, 34, 23, 40, 15, 24, 19, 39, 48, 45, 46, 13, 46, 31, 43,
            12, 47, 48, 12, 43, 19, 34, 37, 40, 20, 32, 37, 39, 46, 24, 42, 31,
            43, 22, 32, 23, 11, 14, 24, 21, 11, 34, 20, 31, 35, 19, 14, 27, 23,
            34, 28, 33, 49, 37, 23, 33, 14, 28, 42, 46, 10, 50, 47, 18, 28, 31,
            45, 46, 33, 35, 38, 15, 45, 14, 19, 29, 32, 26, 29, 10, 50, 39, 50,
            41, 19, 23, 17, 48, 39, 49, 16, 19, 41, 12, 32, 12, 20, 33, 27, 50,
            46, 18, 18, 39, 29, 29, 45, 47, 27, 37, 18, 40, 17, 33, 38, 20, 18,
            22, 34, 13, 27, 13, 15, 30, 17, 19, 49, 14, 24, 40, 48, 11, 10, 36,
            46, 26, 40, 14, 43, 11, 25, 37, 44, 23, 25, 23, 32};
  buy = {11, 19, 19, 17, 14, 10, 12, 11, 15, 18, 20, 10, 11, 20, 16, 19, 20,
         15, 11, 13, 14, 17, 16, 19, 12, 11, 12, 13, 11, 19, 17, 11, 11, 18,
         16, 20, 10, 20, 20, 13, 12, 19, 10, 13, 16, 15, 20, 10, 13, 17, 18,
         17, 19, 14, 20, 13, 15, 13, 13, 13, 15, 11, 19, 17, 15, 15, 18, 14,
         13, 17, 10, 16, 14, 11, 13, 16, 10, 19, 20, 20, 11, 10, 16, 15, 15,
         11, 11, 17, 20, 14, 20, 13, 20, 19, 15, 13, 14, 12, 15, 14};
  free = {11, 15, 18, 17, 20, 13, 12, 13, 14, 11, 14, 15, 19, 13, 19, 16, 15,
          15, 14, 18, 18, 10, 10, 17, 19, 12, 11, 18, 18, 20, 13, 19, 14, 20,
          17, 13, 16, 16, 12, 20, 12, 12, 19, 19, 20, 19, 18, 13, 16, 14, 13,
          12, 12, 18, 14, 11, 10, 15, 15, 10, 16, 10, 12, 20, 20, 15, 11, 15,
          12, 15, 10, 16, 20, 10, 11, 16, 13, 20, 16, 10, 20, 13, 12, 17, 13,
          20, 17, 18, 13, 14, 12, 12, 17, 14, 13, 18, 12, 16, 20, 19};
  coupons = convert(buy, free);
  res = pizzaProblem->miniCost(pizzas, coupons);
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  cout << "Test 26 Correct answer: N/A, our answer: " << res
       << " Time: " << duration << endl;
  return 0;
}