#include <iostream>
#include <bits/stdc++.h>
#define ll long long int
#define ld long double
using namespace std;
 
class SegmentTree{
  struct Node{
    int left, right;
    ll sum, delta, mn, mx;
  };
  public:
     vector<Node> tree;
     int n;
     vector<ll> a;
     SegmentTree(vector<ll> b){
        n = b.size();
        tree.resize(4*n+1);
        a = b;
        build(1, 0, n-1);       
     }
     void range_add(int l, int r, ll dt){
       update_range(1, l, r, dt);
     }
     ll range_sum(int l, int r){
        return sum_range(1, l, r, 0ll);
     }
 
	 ll range_min(int l, int r){
		return get_val(1, l, r, 0ll, false);
	 }
 
	 ll range_max(int l, int r){
		return get_val(1, l, r, 0ll,  true);
	 }
 
  private:
     void build(int u, int l, int r){
       tree[u].left = l;
       tree[u].right = r;
       tree[u].delta = 0ll;
       if (l == r){
         tree[u].sum = a[l];
		 tree[u].mn = a[l];
		 tree[u].mx = a[l];
         return;
       }
       int mid = (l + r) / 2;
       build(2*u, l, mid);
       build(2*u+1, mid+1, r);
       tree[u].sum = tree[2*u].sum + tree[2*u+1].sum;
	   tree[u].mn = min(tree[2*u].mn, tree[2*u+1].mn);
	   tree[u].mx = max(tree[2*u].mx, tree[2*u+1].mx);
     }
  
	 int len(int u){
		return tree[u].right - tree[u].left + 1;
	 }
 
     void update(int u){
       if (tree[u].left != tree[u].right){
		 int l = 2*u, r = l+1;
		 tree[u].mn = min(tree[l].mn+tree[l].delta, tree[r].mn+tree[r].delta);
		 tree[u].mx = max(tree[l].mx+tree[l].delta, tree[r].mx+tree[r].delta);
		 ll lsum = tree[l].sum + tree[l].delta*len(l);
         ll rsum = tree[r].sum + tree[r].delta*len(r);
		 tree[u].sum = lsum+rsum;
       }
     }
     // the corresponding delta will only be stored at the highest level of the range nodes
     // the ranges inside a valid range will not store this delta
     void update_range(int u, int l, int r, ll dt){
		if (l > tree[u].right || tree[u].left > r) return;
		if (l <= tree[u].left && tree[u].right <= r){
			tree[u].delta += dt;
			return;
		}
		update_range(2*u, l, r, dt);
		update_range(2*u+1, l, r, dt);
		update(u);
     }
  
     // prev_delta will store the cumulative deltas applied as we go down the tree
     ll sum_range(int u, int l, int r, ll prev_delta){
        if (l > tree[u].right || tree[u].left > r) return 0ll;
        if (l <= tree[u].left && tree[u].right <= r){
        	return tree[u].sum + (tree[u].delta + prev_delta)*len(u);
        }
        ll curr_delta = prev_delta + tree[u].delta;
        ll left_sum = sum_range(2*u, l, r, curr_delta);
        ll right_sum = sum_range(2*u+1, l, r, curr_delta);
    	 
		return left_sum + right_sum;
     }
 
	 ll get_val(int u, int l, int r, ll prev_delta, bool is_max){
		if (l > tree[u].right || tree[u].left > r){
			if (is_max) return LONG_MIN;
			return LONG_MAX;
		}
		if (l <= tree[u].left && tree[u].right <= r){
			ll dt = tree[u].delta + prev_delta;
        	return (is_max ? tree[u].mx : tree[u].mn) + dt;
		}
 
		ll curr_delta = prev_delta + tree[u].delta;
		ll left_val = get_val(2*u, l, r, curr_delta, is_max);
		ll right_val = get_val(2*u+1, l, r, curr_delta, is_max);
 
		if (is_max) return max(left_val, right_val);
		
		return min(left_val, right_val); 
	 }
    
};






void solve(){
	int n;
	cin >> n;
	vector<ll> a(n);
	for (ll &u : a) cin >> u;
	SegmentTree tree = SegmentTree(a);
	cout << tree.range_sum(0, n-1) << '\n';
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	int tests = 1;
    //cin >> tests;
	while (tests--) solve();
}