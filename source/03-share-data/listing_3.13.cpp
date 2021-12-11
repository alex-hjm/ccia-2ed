#include <map>
#include <string>
#include <mutex>
#include <shared_mutex>

class dns_entry
{};

class dns_cache
{
    std::map<std::string,dns_entry> entries;
    std::shared_mutex entry_mutex;
public:
    dns_entry find_entry(std::string const& domain)
    {
        //使用std::shared_lock<>来保护共享和只读权限
        std::shared_lock<std::shared_mutex> lk(entry_mutex);
        std::map<std::string,dns_entry>::const_iterator const it=
            entries.find(domain);
        return (it==entries.end())?dns_entry():it->second;
    }
    void update_or_add_entry(std::string const& domain,
                             dns_entry const& dns_details)
    {
        //使用std::lock_guard<>实例，当表格需要更新时，为其提供独占访问权限
        //独占锁会阻止其他线程对数据结构进行修改，并且阻止线程调用find_entry()
        std::lock_guard<std::shared_mutex> lk(entry_mutex);
        entries[domain]=dns_details;
    }
};

int main()
{}
