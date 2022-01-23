#pragma once

#include "../../utilities/Utilities.h"

class Node {

public:
    explicit Node(std::string  nodeName) : name(std::move(nodeName)) {};

    Node(const int32_t row, const int32_t column) : matrixRow(row), matrixColumn(column) {};

    int32_t size = 0;
    std::string name;
    int32_t matrixRow = -1;
    int32_t matrixColumn = -1;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    std::shared_ptr<Node> up;
    std::shared_ptr<Node> down;
    std::shared_ptr<Node> header;

};

