#pragma once

class TContext {
  public:
    int threadId = 0;
    TContext() {};
    TContext(int _id) : threadId{_id} {};
};

