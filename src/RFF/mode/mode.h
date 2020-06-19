#ifndef MODE_H_INC
#define MODE_H_INC

namespace FFS {
    class Mode {
        protected:
            std::string name;

        public:
            Mode(std::string _name) : name{_name} {};
            virtual ~Mode() {};
    };
}

#endif
