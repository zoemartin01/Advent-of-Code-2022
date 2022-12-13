#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <set>
#include <tuple>

class Point
{
public:
    size_t x;
    size_t y;
    size_t height;

    Point(size_t _x, size_t _y, size_t _height)
    {
        x = _x;
        y = _y;
        height = _height;
    }

    bool operator==(Point p) const
    {
        return x == p.x && y == p.y;
    }

    void print() const
    {
        std::cout << "(x=" << x << ", y=" << y << ", h=" << height << ")\n";
    }
};

template <typename T>
using Matrix = std::vector<std::vector<T>>;

template <typename T>
std::vector<T> &get_row(Matrix<T> &matrix, size_t row)
{
    return matrix.at(row);
}

template <typename T>
std::vector<T> &get_column(Matrix<T> &transposed, size_t col)
{
    return transposed.at(col);
}

template <typename T>
T *get_point(Matrix<T> &matrix, size_t x, size_t y)
{
    return &matrix.at(y).at(x);
}

std::pair<Matrix<Point>, Matrix<Point>> parse()
{
    Matrix<Point> matrix;
    Matrix<Point> transposed;
    std::ifstream file("input.txt");
    std::string line;
    while (std::getline(file, line))
    {
        std::vector<Point> row;
        for (char c : line)
        {
            if (transposed.size() <= row.size())
                transposed.emplace_back();
            transposed.at(row.size()).emplace_back(row.size(), matrix.size(), c - '0');
            row.emplace_back(row.size(), matrix.size(), c - '0');
        }
        matrix.push_back(row);
    }

    return {matrix, transposed};
}

std::pair<size_t, size_t> get_max_indices(std::vector<Point> vector, size_t left_border, size_t right_border)
{
    auto max = [](const Point &p1, const Point &p2)
    { return p1.height < p2.height; };
    auto first_it = std::max_element(vector.begin(), vector.begin() + left_border, max);
    auto last_it = std::max_element(vector.rbegin(), vector.rend() - right_border - 1, max);

    size_t first_index = first_it - vector.begin();
    size_t last_index = last_it - vector.rbegin();

    return {first_index, vector.size() - 1 - last_index};
}

size_t part_one(Matrix<Point> &input, Matrix<Point> transposed)
{
    std::set<Point *> visible;
    size_t height = input.size();
    size_t width = input.at(0).size();
    for (size_t y = 0; y < height; y++)
    {
        std::vector<Point> row = get_row(input, y);
        size_t left_border = row.size();
        size_t right_border = 0;

        while (left_border != 0 || right_border != row.size() - 1)
        {
            auto [l, r] = get_max_indices(row, left_border, right_border);

            left_border = std::min(l, left_border);
            right_border = std::max(r, right_border);

            auto lp = get_point(input, l, y);
            auto rp = get_point(input, r, y);

            visible.insert(lp);
            visible.insert(rp);
        }
    }

    for (size_t x = 0; x < width; x++)
    {
        std::vector<Point> column = get_column(transposed, x);
        size_t left_border = column.size();
        size_t right_border = 0;

        while (left_border != 0 || right_border != column.size() - 1)
        {
            auto [l, r] = get_max_indices(column, left_border, right_border);

            left_border = std::min(l, left_border);
            right_border = std::max(r, right_border);

            auto lp = get_point(input, x, l);
            auto rp = get_point(input, x, r);

            visible.insert(lp);
            visible.insert(rp);
        }
    }

    return visible.size();
}

size_t calc_score(Point &p, Matrix<Point> input)
{
    const auto &[x, y, height] = p;
    size_t matrix_width = input.size();
    size_t matrix_height = input.at(0).size();
    size_t distance = 1;
    size_t counter = 1;
    for (int i = x - 1; i >= 0; i--, counter++)
    {
        if (get_point(input, i, y)->height >= height)
        {
            counter++;
            break;
        }
    }
    distance *= counter - 1;
    counter = 1;
    for (size_t i = x + 1; i < matrix_width; i++, counter++)
    {
        if (get_point(input, i, y)->height >= height)
        {
            counter++;
            break;
        }
    }
    distance *= counter - 1;
    counter = 1;
    for (int i = y - 1; i >= 0; i--, counter++)
    {
        if (get_point(input, x, i)->height >= height)
        {
            counter++;
            break;
        }
    }
    distance *= counter - 1;
    counter = 1;
    for (size_t i = y + 1; i < matrix_height; i++, counter++)
    {
        if (get_point(input, x, i)->height >= height)
        {
            counter++;
            break;
        }
    }
    distance *= counter - 1;
    return distance;
}

size_t part_two(Matrix<Point> &input, Matrix<Point> &transposed)
{
    size_t height = input.size();
    size_t width = input.at(0).size();
    size_t highest = 0;
    for (size_t x = 0; x < width; x++)
    {
        auto column = get_column(transposed, x);
        for (size_t y = 0; y < height; y++)
        {
            auto row = get_row(input, y);
            Point point = *get_point(input, x, y);

            size_t score = calc_score(point, input);
            highest = score > highest ? score : highest;
        }
    }

    return highest;
}

int main()
{
    auto [input, transposed] = parse();
    std::cout << "Part One: " << part_one(input, transposed) << std::endl;
    std::cout << "Part Two: " << part_two(input, transposed) << std::endl;
}
