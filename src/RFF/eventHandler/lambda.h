#ifndef LAMBDA_H_INC
#define LAMBDA_H_INC

// From : https://stackoverflow.com/questions/7852101
struct Lambda {

	template<typename Tret = void, typename Tfp = Tret(*)(void*), typename T>
	static Tfp ptr(T& t) {
		fn<T> (&t);
		return (Tfp) lambda_ptr_exec<Tret, T>;
	}

	template<typename T>
	static void* fn(void* new_fn = nullptr) {
		static void* fn;
		if(new_fn != nullptr) {
			fn = new_fn;
		}
		return fn;
	}

	template<typename Tret, typename T>
	static Tret lambda_ptr_exec(void* data) {
		auto fn_u = fn<T>();
		return (Tret)(* (T*) fn_u)(data);
	}


};

#endif
