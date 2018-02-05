class JSDOSA {
    public:
    int a;

    int get() { return a; }
    void set(int aa) { a = aa; }

    template<typename _Callable, typename... _Args>
	void enqueue(_Callable&& __f, _Args&&... __args)
	{
        int a  = 3;
        int b = a + 4;
		return;
	}
};