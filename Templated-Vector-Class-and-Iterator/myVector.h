#ifndef MY_VECTOR_H
#define MY_VECTOR_H
//Fall 2023 CS204 HW#5 -- myVector.h

/*******************************************************************************************************
    IMPLEMENT FOLLOWING CLASSES
    YOU CAN ADD ANY FUNCTION/STRUCT/CLASS THAT YOU WILL USE HOWEVER YOU CANNOT TOUCH MAIN FUNCTION
*******************************************************************************************************/
//T1 = value, T2 = key
template <typename T1, typename T2>
class myVector {
public:
    // Default constructor
    myVector() {}

    //Copy constructor
    myVector(const myVector& other) :elements(other.elements) {}

    //Destructor
    ~myVector() {}

    //Set name of vector
    void setName(std::string n) {
        this->name = n;
    }

    //Push_back
    void push_back(const T1& val, const T2& k) {
        this->elements.push_back(std::make_pair(val, k));
    }



    // Method to find a value by key using operator[]
    T1* operator[] (const T2& key) {
        for (auto& element : elements) {
            if (element.second == key) {
                return &element.first;
            }
        }
        return nullptr; // Return nullptr if key is not found
    }

    //Remove element
    void remove(const T2& key) {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (it->second == key) {
                elements.erase(it);
                return; // Exit the function after removing the element
            }
        }
    }

    // Copy vector to another vector
    myVector& operator=(const myVector& rhs) {
        if (this != &rhs) { // Check for self-assignment
            elements.clear(); // Clear the current elements
            for (auto& element : rhs.elements) {
                this->push_back(element.first, element.second);
            }
        }
        return *this; // Return a reference to the current object
    }

    //Check if two vectors are equal
    bool operator==(const myVector& rhs) const {
        if (elements.size() != rhs.elements.size()) {
            return false; // Vectors are not equal if their sizes are different
        }
        for (size_t i = 0; i < elements.size(); ++i) {
            if (elements[i].first != rhs.elements[i].first || elements[i].second != rhs.elements[i].second) {
                return false; // Vectors are not equal if their elements are different
            }
        }
        return true; // Vectors are equal if all elements are the same
    }

    //isEmpty
    bool isEmpty() const {
        return elements.empty(); // Returns true if the vector is empty, false otherwise
    }

    //getName
    std::string  getName() const {
        return name;
    }

    //Processing the data according to the value type of the vector :
    //If the value is string : calculate most frequent word and character for all values of the vector.
    //    If the value is arithmetic : calculate the mean, median, standard deviation, maxand mode values
    //    for all values of the vector.

    void process_data() const {
        if constexpr (std::is_same<T1, std::string>::value) {
            // Process string data
            // Use arrays or custom data structures to count word and character frequencies
            // Find the most frequent word and character
            process_string_data();
        }
        else if constexpr (std::is_arithmetic<T1>::value) {
            // Process arithmetic data
            // Calculate mean, median, standard deviation, max, and mode
            // Implement custom sorting and square root functions if needed
            process_arithmetic_data();
        }
    }

    void process_string_data() const {
        struct WordFreq {
            std::string word;
            int frequency;
        };

        std::vector<WordFreq> wordFreqs;
        std::string mostFrequentWord;
        int maxWordFrequency = 0;
        char mostFrequentChar = '\0';
        int maxCharFrequency = 0;
        int charFrequencies[256] = { 0 }; // Assuming ASCII characters

        // Count word frequencies
        for (const auto& element : elements) {
            bool found = false;
            for (auto& wf : wordFreqs) {
                if (wf.word == element.first) {
                    wf.frequency++;
                    found = true;
                    break;
                }
            }
            if (!found) {
                wordFreqs.push_back({ element.first, 1 });
            }
        }

        // Sort words by frequency and lexicographic order
        for (size_t i = 0; i < wordFreqs.size(); ++i) {
            for (size_t j = i + 1; j < wordFreqs.size(); ++j) {
                if (wordFreqs[j].frequency > wordFreqs[i].frequency ||
                    (wordFreqs[j].frequency == wordFreqs[i].frequency && wordFreqs[j].word < wordFreqs[i].word)) {
                    std::swap(wordFreqs[i], wordFreqs[j]);
                }
            }
        }

        if (!wordFreqs.empty()) {
            mostFrequentWord = wordFreqs.front().word;
            maxWordFrequency = wordFreqs.front().frequency;
        }

        // Count character frequencies
        for (const auto& element : elements) {
            for (char ch : element.first) {
                charFrequencies[static_cast<unsigned char>(ch)]++;
            }
        }

        // Find the most frequent character
        for (int i = 0; i < 256; ++i) {
            if (charFrequencies[i] > maxCharFrequency ||
                (charFrequencies[i] == maxCharFrequency && i < static_cast<int>(mostFrequentChar))) {
                mostFrequentChar = static_cast<char>(i);
                maxCharFrequency = charFrequencies[i];
            }
        }

        std::cout << "Most frequent character: '" << mostFrequentChar << "' (Frequency: " << maxCharFrequency << ")\n";
        std::cout << "Most frequent word: \"" << mostFrequentWord << "\" (Frequency: " << maxWordFrequency << ")\n";
    }


    void process_arithmetic_data() const {
        // Variables to calculate mean, median, standard deviation, max, and mode
        T1 sum = 0;
        T1 max = elements[0].first;
        T1 mode = elements[0].first;
        int modeFrequency = 0;
        std::vector<T1> values;

        // Calculate sum and find max value
        for (const auto& element : elements) {
            sum += element.first;
            if (element.first > max) {
                max = element.first;
            }
            values.push_back(element.first);
        }

        // Calculate mean
        double mean = static_cast<double>(sum) / elements.size();

        // Sort values to calculate median
        custom_sort(values.begin(), values.end());

        // Calculate median
        T1 median = values.size() % 2 == 0 ? (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2
            : values[values.size() / 2];

        // Calculate standard deviation
        double varianceSum = 0;
        for (const auto& value : values) {
            varianceSum += (static_cast<double>(value) - mean) * (static_cast<double>(value) - mean);
        }
        double variance = varianceSum / values.size();
        double standardDeviation = custom_sqrt(variance);

        // Find mode
        for (const auto& value : values) {
            int frequency = std::count(values.begin(), values.end(), value);
            if (frequency > modeFrequency) {
                mode = value;
                modeFrequency = frequency;
            }
        }

        std::cout << "Mean: " << mean << "\n";
        std::cout << "Median: " << median << "\n";
        std::cout << "Standard Deviation: " << standardDeviation << "\n";
        std::cout << "Max: " << max << "\n";
        std::cout << "Mode: " << mode << "\n";
    }

    void custom_sort(typename std::vector<T1>::iterator begin, typename std::vector<T1>::iterator end) const {
        if (begin == end) return; // If the vector is empty, return immediately

        for (auto i = begin + 1; i != end; ++i) {
            auto key = *i;
            auto j = i - 1;

            // Move elements of the vector that are greater than key to one position ahead of their current position
            while (j >= begin && *j > key) {
                *(j + 1) = *j;
                if (j == begin) break; // Prevent decrementing j before begin
                --j;
            }
            *(j + 1) = key;
        }
    }


    // Custom square root function using Newton's method
    double custom_sqrt(double value) const {
        double x = value;
        double last_x = 0;
        while (x != last_x) {
            last_x = x;
            x = (x + value / x) / 2;
        }
        return x;
    }


    class Iterator {
    public:
        // Parametric constructor now takes a reference to myVector
        Iterator(myVector& vec) : vectorRef(vec), current(vec.elements.begin()) {}

        //Destructor
        ~Iterator() {}

        // Initialise
        void Init() {
            current = vectorRef.elements.begin(); // Initialize 'current' to the beginning of the vector
        }

        bool HasMore()  //Return true if has more, false otherwise
        {
            return current != vectorRef.elements.end();
        }

        std::pair<T1, T2>& Current() { // Return the current element
            return *current;
        }

        void Next() // next
        {
            current++;
        }
        //Print entire vector
        void printVector() {
            //myVector<std::string, int>::Iterator iter(vector.elements.begin());
            for (Init(); HasMore(); Next()) {
                std::cout << "Value: " << Current().first << ", Key: " << Current().second << std::endl;
            }

        }


        // Finds an element by key.
        bool find(const T2& key, const myVector& vec) {
            vectorRef = vec;
            for (current = vectorRef.elements.begin(); current != vectorRef.elements.end(); ++current) {
                if (current->second == key) {
                    foundElement = current;
                    return true;
                }
            }
            return false;
        }

        //Prints individual element
        void print(const myVector& vector, const T2& key) const {

            std::cout << foundElement->first << ", " << foundElement->second << std::endl;

        }

        // Replace oldkey with newkey
        void replaceKey(T2 newkey) {
            if (foundElement != vectorRef.elements.end()) {
                foundElement->second = newkey;
            }
        }

    private:
        myVector& vectorRef; // Reference to the myVector object
        typename std::vector<std::pair<T1, T2>>::iterator current; // Points to the current element in the myVector
        typename std::vector<std::pair<T1, T2>>::iterator foundElement; // Stores the iterator of the found element

    };

    Iterator begin() {
        return Iterator(*this); // Return an Iterator object initialized with this myVector instance
    }
private:
    std::string name;
    std::vector<std::pair<T1, T2>> elements; // Stores pairs of keys (T1) and values (T2)
};
/*******************************************************************************************************/
#endif