#ifndef SHARED_PTR_HEADER
#define SHARED_PTR_HEADER
#include <utility>
#include <iostream>

class ControlBlockBase {
public:
    ControlBlockBase() : m_refCount(0) {}

    // dtor is virtual, so that we can call derived class's dtor from a ptr to this base class.
    virtual ~ControlBlockBase() {}; // TODO: implement the destructor.

    // pure virtual function; must be overriden by derived classes
    virtual void* managedAddress() = 0;

    // Delete copies, which also implicitly deletes moves.
    ControlBlockBase(const ControlBlockBase&) = delete;
    ControlBlockBase& operator=(const ControlBlockBase&) = delete;

    long increment()
    {
        return ++m_refCount;
    }

    long decrement()
    {
        return --m_refCount; // what if its already 0?
    }

    long refCount() const
    {
        return m_refCount;
    }

private:
    long m_refCount;
};

template <typename T>

class ControlBlock : public ControlBlockBase {

     T*  ptr;

    public:

    ControlBlock(T* ptr) : ptr(ptr) {}

    ~ControlBlock() override {
        delete ptr;
    }

    void* managedAddress() override {
        return ptr;
    }


};

template <typename T>
class ControlBlockEmbedded : public ControlBlockBase {
    T resource;
 public:

    template<typename... Args>
    ControlBlockEmbedded(Args&&... args) : resource(std::forward<Args>(args)...) {}

    ~ControlBlockEmbedded() override {}
    void* managedAddress() override {return &resource;}



};

template <typename T>
class SharedPtr;

template <typename T, typename... Args> 
SharedPtr<T> makeShared(Args&&... args);

template <typename T>
class SharedPtr {



    T* strd_ptr;
    ControlBlockBase* ctrl_blk;

    explicit SharedPtr(T* ptr, ControlBlockBase* ctrl_blk_base) : strd_ptr(ptr), ctrl_blk(ctrl_blk_base) {}

    public:
    
    template <typename U,typename... Args>  friend SharedPtr<U> makeShared(Args&&... args);

    void swap(SharedPtr<T>& other) {
        std::swap(strd_ptr, other.strd_ptr);
        std::swap(ctrl_blk, other.ctrl_blk);
        return;
    }

    
    


    
    SharedPtr() : strd_ptr(nullptr), ctrl_blk(nullptr) {}

    SharedPtr( T* ptr) : strd_ptr(ptr), ctrl_blk(new ControlBlock<T>(ptr)) {
        ctrl_blk->increment(); // 
    }

    SharedPtr(const SharedPtr<T>& lvalue) : strd_ptr(lvalue.strd_ptr), ctrl_blk(lvalue.ctrl_blk){
        ctrl_blk->increment();

    }

    SharedPtr(SharedPtr<T>&& rvalue) : strd_ptr(rvalue.strd_ptr), ctrl_blk(rvalue.ctrl_blk) {
        rvalue.ctrl_blk = nullptr;
        rvalue.strd_ptr = nullptr;

    }

    SharedPtr<T>& operator=( const SharedPtr& lvalue) {

        auto tmp = lvalue;
        swap(tmp);
        return *this;
        
    }

    SharedPtr<T>& operator=(SharedPtr&& rvalue) {
        swap(rvalue);
        return *this;
    }

    ~SharedPtr() {
        if (ctrl_blk){
        ctrl_blk->decrement();
        if (!ctrl_blk->refCount()) delete ctrl_blk; // delete the ctrl block of nobody has ownership
        }
    }

    T& operator*() {return *strd_ptr;}
    T* operator->() const {return strd_ptr;}

    T* get() const {return strd_ptr;}
    bool operator==(const SharedPtr<T>& other) const {
        return strd_ptr == other.strd_ptr;
    }
    bool operator!=(const SharedPtr<T>& other) const {
        return !(*this == other);
    }

    operator bool() const {
        return (strd_ptr != nullptr);
    }

    void reset() {
       swap(*this, SharedPtr<T>());
    }

    void reset(T* other) {
        swap(*this, SharedPtr<T>(other));
    }

    long useCount() {return ctrl_blk->refCount();}




};




template <typename T, typename... Args> 
SharedPtr<T> makeSharedBasic(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}


template <typename T, typename... Args> 
SharedPtr<T> makeShared(Args&&... args) {
    auto* ctrl_blk_emb_ptr = new ControlBlockEmbedded<T>(std::forward<Args>(args)...);

    return SharedPtr<T>(static_cast<T*>(ctrl_blk_emb_ptr->managedAddress()), ctrl_blk_emb_ptr);

}


#endif
