#pragma once

#include <stdexcept>
#include <utility>

#include "parallel.h"

using par::sequence;


// Do not modify these structs!
struct building {
  int l, h, r;
};

struct coord {
  int x, y;
};

sequence<coord> auto singleton(const building& b) {
  return par::array_sequence<coord>{coord{b.l, b.h}, coord{b.r, 0}};
}


sequence<coord> auto combine(const sequence<coord> auto& s1, const sequence<coord> auto& s2) {
  auto&& s2_mapped = par::map(s2,[&](auto&& a){return coord{a.x,-1};});
  auto&& s1_merged = par::merge(s1,s2_mapped,[&](auto&& a,auto&& b){return a.x < b.x;});
  auto&& s1_height = par::map(s1_merged,[&](auto&& a){return a.y;});
  auto&& s1_added = par::scan_inclusive(s1_height, -1, 
                                       [&](auto&& a, auto&& b){
                                           if(b != -1) return b;
                                           else return a;
                                       });
  //propagate height with copy right

  auto&& s1_mapped = par::map(s1,[&](auto&& a){return coord{a.x,-1};});
  auto&& s2_merged = par::merge(s2,s1_mapped,[&](auto&& a,auto&& b){return a.x < b.x;});
  auto&& s2_height = par::map(s2_merged,[&](auto&& a){return a.y;});
  auto&& s2_added = par::scan_inclusive(s2_height, -1, 
                                       [&](auto&& a, auto&& b){
                                           if(b != -1) return b;
                                           else return a;
                                       });

  auto&& merged = 
        par::tabulate(s1_merged.size(),
                      [&](auto i){
                      return coord{s1_merged[i].x,std::ranges::max(s1_added[i],s2_added[i])};
                      });

  auto&& res = 
        par::filter_idx(merged,
                        [&](size_t i, auto e){
                          if(i==0) return true;
                          else return (e.y != merged[i-1].y);
                        });
  
  return res;
}
