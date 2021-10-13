#pragma once
#include <string>
#include <vector>

using namespace std;

typedef int (*MetricFunction)(const string&, const string&);

class VPTree {
    private:
        vector<string> data;
        vector<unsigned> outer;
        vector<int> radius;
        vector<bool> has_inner;
        MetricFunction dist = nullptr;

        void init_self(vector<string>::iterator, vector<string>::iterator, const int);
        vector<string> _findNearest(const string&, int, const int) const;
    public:
        VPTree(vector<string>&&, MetricFunction);
        
        vector<string> findNearest(const string&, int) const;
        MetricFunction getDist();
};
