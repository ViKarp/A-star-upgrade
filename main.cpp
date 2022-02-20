#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <utility>


auto fast_push(std::vector<std::pair<std::pair<int, int>, int>> vector1, std::pair<int, int> &pair, int &i, int cheb){
    auto it = vector1.cbegin();
    long length = vector1.size();
    auto f = i + cheb;
    if (length == 0) {
        vector1.push_back(std::pair <std::pair<int,int>,int> (pair,f));
    }
    else {
        bool flag = true;
        for (int i = 0; i < length; i++) {
            if (vector1[i].second > f) {
                flag = false;
                vector1.insert(it+i, std::pair <std::pair<int,int>,int> (pair,f));
                break;
            }
        }
        if (flag) {
            vector1.push_back(std::pair <std::pair<int,int>,int> (pair,f));
        }
    }
    return vector1;
}

int height_diff(std::pair<int, int> pair1, std::pair<int, int> &pair2, std::vector<std::vector<int>> height){
    int diff = height[pair2.first][pair2.second]-height[pair1.first][pair1.second];
    if (diff<=0){
        return 0;
    }
    else {
        return diff;
    }
};

inline bool space(char c){
    return std::isspace(c);
}

inline bool notspace(char c){
    return !std::isspace(c);
}

std::vector<int> split(const std::string& s) {
    typedef std::string::const_iterator iter;
    std::vector<int> ret;
    iter i = s.begin();
    while (i != s.end()) {
        i = std::find_if(i, s.end(), notspace);
        iter j = std::find_if(i, s.end(), space);
        if (i != s.end()) {
            ret.push_back(stoi(std::string(i, j)));
            i = j;
        }
    }
    return ret;
}

auto point_search(std::pair <int,int> current, std::vector <std::pair <int,int>> verified) {
    for (int i = 0; i<verified.size();i++){
        if (current == verified[i]){
            return (std::pair <bool, int> (true, i));
        }
    }
    return (std::pair <bool, int> (false, 0));
}
auto point_search1(std::pair <std::pair<int,int>, int> current, std::vector <std::pair<std::pair<int,int>,int>> verified) {
    for (int i = 0; i<verified.size();i++){
        if (current.first == verified[i].first){
            return std::pair <bool,int> (true,i);
        }
    }
    return std::pair <bool,int> (false,-1);
}
auto point_search2(std::pair <int,int> current, std::vector <std::pair<std::pair<int,int>,int>> verified) {
    for (int i = 0; i < verified.size(); i++) {
        if (current == verified[i].first) {
            return true;
        }
    }
    return false;
}
auto cheb(std::pair <int,int> u, std::pair <int,int> finish,std::vector<std::vector<int>> height){
    int dif;
    if (height[u.first][u.second]-height[finish.first][finish.second]<=0) {
        dif = abs(height[u.first][u.second]-height[finish.first][finish.second]);
    }
    else {
        dif = 0;
    }
    return std::max(abs(u.first - finish.first),abs(u.second - finish.second)) + dif;
}
int main() {
    int line, width, x, y, a, b, maxlen = 100000;
    int temporary_distance;
    std::vector<std::vector<int>> height;
    std::ifstream fin;
    fin.open("height.txt");
    std::cout << "What is size map?\n";
    std::cin >> a >> b;
    std::string buffer;
    int i = 0;
    while (i != a) {
        getline(fin,buffer);
        height.push_back(split(buffer));
        i++;
    }
    fin.close();


    fin.open("tree.txt");
    std::vector<std::vector<int>> tree;
    i = 0;
    while (i != a) {
        getline(fin,buffer);
        tree.push_back(split(buffer));
        i++;
    }
    fin.close();


    std::cout << "What is coordinates of start point?\n";
    std::cin >> x >> y;
    std::pair <int,int> start_point  (x,y);
    std::cout << "What is coordinates of stop point?\n";
    std::cin >> x >> y;
    std::pair <int,int> stop_point  (x,y);
    bool inverse = false;
    std::cout << "Is the corridor vertical? 1 or 0\n";
    std::cin >> inverse;
    std::cout << "What is the width of the corridor?\n";
    std::cin >> width;
    if ((width > a and not inverse) or (width > b and inverse)) {
        while ((width > a and not inverse) or (width > b and inverse)) {
            std::cout << "Oh no! Width more then size. Enter new width.\n";
            std::cin >> width;
        }
    }
    std::cout << "What is the first line in corridor?\n";
    std::cin >> line;
    if (((line<0 or line >a-1) and not inverse) or (inverse and (line < 0 or line > b-1))) {
        while (((line<0 or line >a-1) and not inverse) or (inverse and (line < 0 or line > b-1))) {
            std::cout << "First line is not in map! Enter new first line!";
            std::cin >> line;
        }
    }
    if (not inverse and (start_point.first > line+width-1 or start_point.first < line) or inverse and (start_point.second > line+width-1 or start_point.second < line)){
        std::cout << "Error!!!You are not in the corridor!";
        exit;
    }
    if (not inverse and (stop_point.first > line+width-1 or stop_point.first < line) or inverse and (stop_point.second > line+width-1 or stop_point.second < line)){
        while (not inverse and (stop_point.first > line+width-1 or stop_point.first < line) or inverse and (stop_point.second > line+width-1 or stop_point.second < line)){
            std::cout << "Error!Your stop point are not in the corridor! Enter new coordinates.\n";
            std::cin >> x >> y;
            stop_point.first = x;
            stop_point.second = y;
        }
    }


    std::vector <std::pair<std::pair<int,int>,int>> queue;
    std::vector <std::pair<int,int>> verified;
    std::vector <std::vector<int>> distance;
    distance.resize(a);
    for (i = 0; i < a;i++) {
        distance[i].resize(b);
    }
    distance[start_point.first][start_point.second] = tree[start_point.first][start_point.second]*5;
    std::vector <std::vector<std::pair <int,int>>> parent;
    parent.resize(a);
    for (i = 0; i < a;i++) {
        parent[i].resize(b);
    }
    queue.push_back(std::pair(start_point,tree[start_point.first][start_point.second]*5 +cheb(start_point,stop_point, height)));
    while (queue.size()!=0){
        auto current = queue[0];
        if (current.first == stop_point) {
            break;
        }
        queue.erase(queue.begin());
        verified.push_back(current.first);
        std::vector <std::pair <int,int>> region;
        auto cur_x = current.first.first;
        auto cur_y = current.first.second;
        {//sorry for that....
            if (not inverse and (cur_x-1>=line and cur_x-1<=line+width-1 and cur_y>=0 and cur_y<=b-1)
            or inverse and (cur_x-1>=0 and cur_x-1<=a-1 and cur_y>=line and cur_y <= line+width-1)) {
                region.push_back(std::pair<int, int>(cur_x - 1, cur_y));
            }
            if (not inverse and (cur_x+1>=line and cur_x+1<=line+width-1 and cur_y>=0 and cur_y<=b-1)
            or inverse and (cur_x+1>=0 and cur_x+1<=a-1 and cur_y>=line and cur_y <= line+width-1)) {
                region.push_back(std::pair<int, int>(cur_x + 1, cur_y));
            }
            if (not inverse and (cur_x+1>=line and cur_x+1<=line+width-1 and cur_y+1>=0 and cur_y+1<=b-1)
            or inverse and (cur_x+1>=0 and cur_x+1<=a-1 and cur_y+1>=line and cur_y+1 <= line+width-1)) {
                region.push_back(std::pair<int, int>(cur_x + 1, cur_y + 1));
            }
            if (not inverse and (cur_x+1>=line and cur_x+1<=line+width-1 and cur_y-1>=0 and cur_y-1<=b-1)
            or inverse and (cur_x+1>=0 and cur_x+1<=a-1 and cur_y-1>=line and cur_y-1 <= line+width-1)) {
                region.push_back(std::pair<int, int>(cur_x + 1, cur_y - 1));
            }
            if (not inverse and (cur_x>=line and cur_x<=line+width-1 and cur_y-1>=0 and cur_y-1<=b-1)
            or inverse and (cur_x+1>=0 and cur_x+1<=a-1 and cur_y>=line and cur_y <= line+width-1)) {
                region.push_back(std::pair<int, int>(cur_x, cur_y - 1));
            }
            if (not inverse and (cur_x>=line and cur_x<=line+width-1 and cur_y+1>=0 and cur_y+1<=b-1)
            or inverse and (cur_x>=0 and cur_x<=a-1 and cur_y+1>=line and cur_y+1 <= line+width-1)) {
                region.push_back(std::pair<int, int>(cur_x, cur_y + 1));
            }
            if (not inverse and (cur_x-1>=line and cur_x-1<=line+width-1 and cur_y-1>=0 and cur_y-1<=b-1)
            or inverse and (cur_x-1>=0 and cur_x-1<=a-1 and cur_y-1>=line and cur_y-1 <= line+width-1)) {
                region.push_back(std::pair<int, int>(cur_x - 1, cur_y - 1));
            }
            if (not inverse and (cur_x-1>=line and cur_x-1<=line+width-1 and cur_y+1>=0 and cur_y+1<=b-1)
            or inverse and (cur_x-1>=0 and cur_x-1<=a-1 and cur_y+1>=line and cur_y+1 <= line+width-1)) {
                region.push_back(std::pair<int, int>(cur_x - 1, cur_y + 1));
            }
        }
        for (auto k = 0;k < region.size();k++) {
            temporary_distance = distance[current.first.first][current.first.second] + 1 + height_diff(current.first,region[k],height) + tree[region[k].first][region[k].second]*5;
            if ((point_search(region[k],verified).first) and temporary_distance >= distance[region[k].first][region[k].second]){
                continue;
            }
            if ((not point_search2(region[k],queue)) or ((point_search(region[k],verified).first) and temporary_distance < distance[region[k].first][region[k].second]) or ((point_search2(region[k],queue)) and temporary_distance < distance[region[k].first][region[k].second])){
                parent[region[k].first][region[k].second] = current.first;
                distance[region[k].first][region[k].second] = temporary_distance;
                if (not point_search1(std::pair <std::pair<int,int>, int> (region[k],temporary_distance),queue).first) {
                    queue = fast_push(queue, region[k], distance[region[k].first][region[k].second], cheb(region[k],stop_point,height));
                }
                else {
                    auto pos = point_search1(std::pair <std::pair<int,int>, int> (region[k],temporary_distance),queue).second;
                    queue[pos].second = distance[region[k].first][region[k].second] + cheb(region[k],stop_point,height);
                }

            }
        }
    }
    auto temp = stop_point;
    std::cout << stop_point.first <<" " << stop_point.second << std::endl;
    while (temp != start_point) {
        std::cout << parent[temp.first][temp.second].first << " " << parent[temp.first][temp.second].second << std::endl;
        temp = parent[temp.first][temp.second];
    }
    std::cout << distance[stop_point.first][stop_point.second];

    return 0;
}


