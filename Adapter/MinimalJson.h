#include <string>

class MinimalJson
{
public:
    virtual ~MinimalJson() = default;
    MinimalJson(const std::string& filename);
    virtual size_t size()
    {
        return 0;
    }
    virtual std::string getElementValue()
    {
        return " ";
    }
};

