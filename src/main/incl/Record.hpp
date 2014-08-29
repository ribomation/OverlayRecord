/*
 * Record.hpp
 *
 *  Created on: Jun 30, 2014
 *      Author: jens
 */

#ifndef RECORD_HPP_
#define RECORD_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <cstring>

namespace ribomation {

    // -----------------------------------------------------
    // --- Exceptions
    // -----------------------------------------------------
    /**
     * Exception thrown when a field gets out of bounds of the record's storage.
     */
    struct StorageOverflow : public std::logic_error {
        StorageOverflow(char* offset, int sz)
                : std::logic_error(
                        "offset=" + std::to_string((unsigned long) offset)
                      + ", sz=" + std::to_string(sz)) {
        }
        StorageOverflow() : std::logic_error("Buffer mismatch") {}
    };

    /**
     * Exception thrown when indexed out of bounds.
     */
    struct IndexOutOfBounds : public std::domain_error {
        IndexOutOfBounds(int ix, int ub)
                : std::domain_error("ix=" + std::to_string(ix) + ", ub=" + std::to_string(ub)) {
        }
    };

    struct UnInitialized : public std::logic_error {
        UnInitialized() : std::logic_error("Have you forgotten to append '= {this};' to your fields") {}
        UnInitialized(std::string msg) : std::logic_error(msg) {}
    };


    // -----------------------------------------------------
    // --- Type Aliases
    // -----------------------------------------------------
    typedef unsigned      FieldSize;


    // -----------------------------------------------------
    // --- class FieldBase
    // -----------------------------------------------------
    class Record; //forward decl
    
    struct FieldBase {
        Record*     record = nullptr;
        unsigned    fieldOffset = 0;
        unsigned    fieldSize   = 0;

        FieldBase() = default;

        unsigned    startOffset() const {
            return fieldOffset;
        }

        unsigned    endOffset() const {
            return startOffset() + fieldSize;
        }

    };

    // -----------------------------------------------------
    // --- class Record
    // -----------------------------------------------------
    /**
     * Primary base class for all mapped-records.
     * Applications should subclass Record and define one or more Field members
     * plus a constructor which provides the storage.
     */
    class Record {
        std::vector<FieldBase*>   fields;
        char*       buffer = nullptr;
        unsigned    bufferSize = 0;
        char*       dynamicBuffer = nullptr;

        void disposeDynamicBuffer() {
            if (dynamicBuffer != nullptr) {
                delete dynamicBuffer;
                dynamicBuffer = nullptr;
                buffer        = nullptr;
            }
        }

    public:
        Record() = default;

        virtual ~Record() {
            disposeDynamicBuffer();
        }

        Record&     allocateDynamicBuffer() {
            dynamicBuffer = new char[ size() ];
            buffer = dynamicBuffer;
            return *this;
        }

        Record&     assignStaticBuffer(char* buf, unsigned bufsiz) {
            if (bufsiz < size()) throw StorageOverflow();
            buffer = buf;
            if (dynamicBuffer != nullptr) disposeDynamicBuffer();
        }

        unsigned    getLastOffset() const {
            if (fields.empty()) return 0;
            auto last = fields.back();
            return last->fieldOffset + last->fieldSize;
        }

        void updateBufferSize() {
            auto result = std::max_element(fields.begin(), fields.end(), [](FieldBase* left, FieldBase* right){
                return left->endOffset() < right->endOffset();
            });
            bufferSize = (*result)->endOffset();
        }

        unsigned size() const {
            if (fields.empty()) throw UnInitialized();
            return bufferSize;
        }

        /**
         * Returns the start address of a record.
         */
        char* begin() const {
            if (buffer == nullptr) throw UnInitialized();
            return buffer;
        }

        /**
         * Returns the end address of a record.
         */
        char* end() const {
            return begin() + size();
        }

        /**
         * Copies buf into storage.
         * It's assumed that sizeof(buf) is sufficient.
         */
        Record& operator <<(char* buf) {
            if (buffer == nullptr) throw UnInitialized();
            std::memcpy(buffer, buf, size());
            return *this;
        }

        /**
         * Replaces storage with buf.
         * It's assumed that sizeof(buf) is sufficient.
         */
        Record&  operator =(char* buf) {
            assignStaticBuffer(buf, size());
            return *this;
        }

        /**
         * Moves <em>n</em> record positions over the underlying buffer.
         * Returns the new start position.
         * It's assumed that the buffer has sufficient size.
         */
        char* operator +=(int n) {
            buffer += n * size();
            return buffer;
        }

        /**
         * Moves one record backward, over the storage.
         * Returns the new start position.
         * It's assumed that the buffer has sufficient size.
         */
        char* operator ++() {
            return *this += +1;
        }

        /**
         * Moves one record forward, over the storage.
         * Returns the new start position.
         * It's assumed that the buffer has sufficient size.
         */
        char* operator --() {
            return *this += -1;
        }


        // -----------------------------------------------------
        // --- class Field
        // -----------------------------------------------------
        /**
         * A record consists of one or more Field members.
         */
        template<typename FieldType, unsigned field_size, typename converter>
        class Field : public FieldBase {


        public:
            typedef FieldType       TYPE;
            static const unsigned  SIZE = field_size;

            void init(Record* record, unsigned offset) {
                this->record      = record;
                this->fieldSize   = field_size;
                this->fieldOffset = offset;
                record->fields.push_back(this);
                record->updateBufferSize();
            }

            /**
             * Let the compiler generate the default constructor
             * for use in array inits.
             */
            Field() = default;

            /**
             * Initializes this field in the given record.
             * @param r   owning record
             */
            Field(Record* record) {
                init(record, record->getLastOffset());
            }

            /**
             * Initializes this field in the given record,
             * using the start-offset of the provided field.
             * @param r   owning record
             * @param f     place this field relative to the field
             * @param alignStart    if true align with beginning of f, else align its end
             */
            template<typename T, unsigned N, typename C>
            Field(Record* record, const Field<T, N, C>& alignField, bool alignStart = true) {
                init(record, alignField.fieldOffset + (alignStart ? 0 : alignField.fieldSize));
            }

            /**
             * Returns its start address.
             */
            char*   begin() const {
                return record->begin() + fieldOffset;
            }

            /**
             * Returns its end address.
             */
            char*   end() const {
                return begin() + size();
            }

            /**
             * Returns its size in number of bytes.
             */
            unsigned size() const {
                return fieldSize;
            }

            /**
             * Returns its value as a string.
             */
            std::string chars() const {
                return std::string(begin(), end());
            }

            /**
             * Sets its value.
             * Uses its converter::toStorage() function.
             * @param v     new value
             */
            void value(FieldType v) {
                converter::toStorage(v, begin(), size());
            }

            /**
             * Returns its value.
             * Uses its converter::fromStorage() function.
             */
            FieldType value() const {
                return converter::fromStorage(begin(), size());
            }

            /**
             * Type conversion operator.
             * Works if the destination type is an exact match.
             * That means, it do not work for <code>char*</code>, unless the destination type is std::string.
             */
            operator FieldType() const {
                return value();
            }
            
            Field<FieldType, field_size, converter>&
            operator =(FieldType v) {
                value(v);
                return *this;
            }
            
        };


        // -----------------------------------------------------
        // --- class Array
        // -----------------------------------------------------
        template<typename ItemType, unsigned numItems>
        class Array {   //TODO: make it a subclass of FieldBase
            ItemType items[numItems];

        public:
            typedef ItemType        TYPE;
            static const unsigned   COUNT = numItems;
            static const unsigned   SIZE = numItems * ItemType::SIZE;

            Array(Record* record) {
                for (int k = 0; k < COUNT; ++k) {
                    items[k].init(record, record->getLastOffset());
                }
            }

            template<typename T, unsigned N, typename C>
            Array(Record* record, const Field<T, N, C>& alignField, bool alignStart = true) {
                if (COUNT < 1) throw IndexOutOfBounds(0, COUNT);

                unsigned offset = alignField.fieldOffset + (alignStart ? 0 : alignField.fieldSize);
                items[0].init(record, offset);
                for (int k = 1; k < COUNT; ++k) {
                    items[k].init(record, record->getLastOffset());
                }
            }

            ItemType& operator[](int ix) {
                if (0 <= ix && ix < numItems) {
                    return items[ix];
                } else {
                    throw IndexOutOfBounds(ix, numItems);
                }
            }

            unsigned size()  const { return SIZE; }
            unsigned count() const { return COUNT; }


            // --- Support of iteration and for-each loops ---
            class iterator {
                Array<TYPE, COUNT>&     arr;
                int                     ix;

            public:
                iterator(Array<TYPE, COUNT>& _arr, int _ix) : arr(_arr), ix(_ix) {}
                TYPE        operator *() { return arr[ix]; }
                iterator&   operator ++() { ++ix; return *this; };
                bool        operator !=(const iterator& that) { return this->ix != that.ix; }
            };
            iterator  begin() { return iterator(*this, 0); }
            iterator  end()   { return iterator(*this, COUNT); }

            // --- Support for array assignment ---
            void  assign(std::initializer_list<typename ItemType::TYPE> values) {
                if (COUNT == values.size()) {
                    auto v = values.begin();
                    for (int k=0; k<COUNT; ++k, ++v) items[k].value( *v );
                } else {
                    throw IndexOutOfBounds(values.size(), numItems);
                }
            }

            void  operator =(std::initializer_list<typename ItemType::TYPE> values) {
                assign(values);
            }

            // --- Support for toString() ---
            std::string  S(typename ItemType::TYPE x, std::true_type) const {
                return std::to_string( x );
            }
            std::string  S(typename ItemType::TYPE x, std::false_type) const {
                return "'" + x + "'";
            }

            std::string  toString(std::string SEP = ", ", std::string LEFT = "[", std::string RIGHT = "]") const {
                std::string  result = S( items[0].value(), std::is_arithmetic<typename ItemType::TYPE>() );

                for (int k=1; k<COUNT; ++k)  result += SEP + S( items[k].value(), std::is_arithmetic<typename ItemType::TYPE>() );
                return LEFT + result + RIGHT;
            }
        };


        // -----------------------------------------------------
        // --- class Embed
        // -----------------------------------------------------
        template<typename RecordType>
        class Embed : public FieldBase {
            RecordType     embeddedRecord;

            void init(Record* record, unsigned offset) {
                this->record      = record;
                this->fieldSize   = embeddedRecord.size();
                this->fieldOffset = offset;
                record->fields.push_back(this);
                record->updateBufferSize();
            }

        public:
            Embed(Record* record) {
                init(record, record->getLastOffset());
            }

            template<typename T, unsigned N, typename C>
            Embed(Record* record, const Field<T, N, C>& alignField, bool alignStart = true) {
                init(record, alignField.fieldOffset + (alignStart ? 0 : alignField.fieldSize));
            }

            RecordType*     operator ->() const {
                return &embeddedRecord;
            }

            /**
             * Returns its start address.
             */
            char*   begin() const {
                return record->begin() + fieldOffset;
            }

            /**
             * Returns its end address.
             */
            char*   end() const {
                return begin() + size();
            }

            /**
             * Returns its size in number of bytes.
             */
            unsigned size() const {
                return fieldSize;
            }

        };


        // -----------------------------------------------------
        // --- Converters
        // -----------------------------------------------------
        template<char PAD = ' '>
        struct TextConverter {
            static void toStorage(std::string& v, char* offset, unsigned size) {
                std::fill_n(offset, size, PAD);
                v.copy(offset, size);
            }

            static std::string fromStorage(char* offset, unsigned size) {
            	std::string  payload(offset, offset + size);
            	std::string  result(size, PAD);
            	for (int k=0; k<payload.size(); ++k) if (payload[k]) result[k] = payload[k];
            	return result;
            }
        };

        template<typename Type, typename str2num, char PAD = ' '>
        struct NumericConverter {
            static void toStorage(Type v, char* offset, size_t size) {
                std::fill_n(offset, size, PAD);
                std::to_string(v).copy(offset, size);
            }

            static Type fromStorage(char* offset, size_t size) {
                str2num f;
                return f(std::string(offset, offset + size));
            }
        };

        template<typename Type>
        struct BinaryConverter {
            static void toStorage(Type v, char* offset, size_t size) {
                std::memcpy(offset, &v, sizeof(Type));
            }

            static Type fromStorage(char* offset, size_t size) {
                Type v = Type(); //=0
                std::memcpy(&v, offset, sizeof(Type));
                return v;
            }
        };


        // -----------------------------------------------------
        // --- Character converter functors
        // -----------------------------------------------------
        struct TO_SHORT {
            short operator()(const std::string& s) {
            	if (s.empty()) return 0;
                try { return static_cast<short>(std::stoi(s)); }
                catch (std::invalid_argument& e) {return 0;}
            }
        };
        struct TO_INT {
            int operator()(const std::string& s) {
            	if (s.empty()) return 0;
                try { return std::stoi(s); }
                catch (std::invalid_argument& e) {return 0;}
            }
        };
        struct TO_LONG {
            long operator()(const std::string& s) {
            	if (s.empty()) return 0;
                try { return std::stol(s); }
                catch (std::invalid_argument& e) {return 0;}
            }
        };
        struct TO_LONG_LONG {
            long long operator()(const std::string& s) {
            	if (s.empty()) return 0;
                try { return std::stoll(s); }
                catch (std::invalid_argument& e) {return 0;}
            }
        };
        struct TO_UNSIGNED_LONG {
            unsigned long operator()(const std::string& s) {
            	if (s.empty()) return 0;
            	try { return std::stoul(s); }
            	catch (std::invalid_argument& e) {return 0;}
            }
        };

        struct TO_FLOAT {
            float operator()(const std::string& s) {
            	if (s.empty()) return 0;
                try { return std::stof(s); }
                catch (std::invalid_argument& e) {return 0;}
            }
        };
        struct TO_DOUBLE {
            double operator()(const std::string& s) {
            	if (s.empty()) return 0;
                try { return std::stod(s); }
                catch (std::invalid_argument& e) {return 0;}
            }
        };
        struct TO_LONG_DOUBLE {
            long double operator()(const std::string& s) {
            	if (s.empty()) return 0;
                try { return std::stold(s); }
                catch (std::invalid_argument& e) {return 0;}
            }
        };


        // -----------------------------------------------------
        // --- Field types
        // -----------------------------------------------------
        template<int size> using Text =
        Field<std::string, size, TextConverter<>>;

        template<int size> using TextInteger =
        Field<int, size, NumericConverter<int, TO_INT>>;

        template<int size> using TextFloat =
        Field<float, size, NumericConverter<float, TO_FLOAT>>;

        template<typename Type> using BinaryType =
        Field<Type, sizeof(Type), BinaryConverter<Type>>;

        using Byte     = BinaryType<char>;
        using Short    = BinaryType<int>;
        using Integer  = BinaryType<int>;
        using Long     = BinaryType<long>;

        using Float    = BinaryType<float>;
        using Double   = BinaryType<double>;
        using Quadruple   = BinaryType<long double>;

        using Word     = BinaryType<short>;
        using QUAD     = BinaryType<long long>;

        template<int size>
        using Blob	   = Field<char, size, TextConverter<>>;
    };

    /**
     * Writes a record binary, to a stream.
     */
    inline std::ostream&  operator <<(std::ostream& os, Record& rec) {
        os.write(rec.begin(), rec.size());
        return os;
    }

    /**
     * Reads a record binary from a stream.
     */
    inline std::istream&  operator >>(std::istream& is, Record& rec) {
        char  buf[rec.size()];
        is.read(buf, rec.size());
        rec << buf;

        return is;
    }

} 

#endif /* RECORD_HPP_ */
