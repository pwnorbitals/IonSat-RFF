#ifndef LAMBDA_H_INC
#define LAMBDA_H_INC

// From : https://stackoverflow.com/questions/7852101
struct Lambda {
    
    template<typename Tret = void, typename Tfp = Tret(*)(void*), typename T>
	static Tfp ptr(T& t) {
        std::cout << "called with t addr = " << &t << std::endl;
		fn<T> (&t);
		return (Tfp) lambda_ptr_exec<Tret, T>;
	}
	
	template<typename T>
	static void* fn(void* new_fn = nullptr) {
		static void* fn;
		if(new_fn != nullptr) {
			fn = new_fn;
            std::cout << "set fn to " << fn << std::endl;
		}
		std::cout << "returning fn = " << fn << std::endl;
		return fn;
	}
	
    template<typename Tret, typename T>
	static Tret lambda_ptr_exec(void* data) {
        auto fn_u = fn<T>();
        std::cout << "using fn = " << fn_u << std::endl;
		return (Tret)(* (T*) fn_u)(data);
	}

	
};

#endif
