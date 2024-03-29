#ifndef EIT_ALGORITHMS_H
#define EIT_ALGORITHMS_H

#include "utility.h"
#include "queue.h"

struct Yard;
struct FileHandler;
struct Ferry;

struct BaseAlgorithm {
    virtual void operator()(Ferry& ferry, Yard& yard, FileHandler& fh, std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel = {}) = 0;
    virtual bool operator()(Yard& yard, Vehicle& vehicle) = 0;
    virtual std::string AlgorithmName() { return "Unset"; }
};

/*
 * General structure of these algorithms:
 * void operator()(Ferry& ferry, std::vector<Queue>& queues, FileHandler& fh) -> fine-sorting algorithm
 * bool operator()(std::vector<Queue>& queues, Vehicle& vehicle) -> coarse-sorting algorithm
 */

/* Worst fit algoritm: Finds the bins with most available space and placed item there
 * Individual selection algorith, not applicable to larger systems
 * Bad algorithm, should not be used, but illustrative
 */
struct WorstFit : BaseAlgorithm {
    // Fine-sorting version of WorstFit algorithm, from Yard queues to Ferry queues
    void operator()(Ferry& ferry, Yard& yard, FileHandler& fh, std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel = {}) override;
    // Coarse-sorting version of WorstFit algorithm, individual car arrival into queues
    bool operator()(Yard& yard, Vehicle& vehicle) override;
    virtual std::string AlgorithmName() { return "Worst fit"; }
};

/* Best fit algoritm: Finds the bins with least available space and placed item there
 * Individual selection algorith, not applicable to larger systems
 * Also not a good algorithm, but illustrative
 */
struct BestFit : BaseAlgorithm{
    // Fine-sorting version of WorstFit algorithm, from Yard queues to Ferry queues
    void operator()(Ferry& ferry, Yard& yard, FileHandler& fh, std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel = {}) override;
    // Coarse-sorting version of WorstFit algorithm, individual car arrival into queues
    bool operator()(Yard& yard, Vehicle& vehicle) override;
    virtual std::string AlgorithmName() { return "Best fit"; }
};

struct BasicRules : BaseAlgorithm{
    void operator()(Ferry& ferry, Yard& yard, FileHandler& fh, std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel = {}) override;
    bool operator()(Yard& yard, Vehicle& vehicle) override;
    virtual std::string AlgorithmName() { return "Hard-coded ruleset"; }
};

struct OptimizeCOM : BaseAlgorithm{
    void operator()(Ferry& ferry, Yard& yard, FileHandler& fh, std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel = {}) override;
    bool operator()(Yard& yard, Vehicle& vehicle) override;
    virtual std::string AlgorithmName() { return "COM ruleset"; }
};

// Algorithm that does nothing, just for testing how the operator() overloading works
// Also illustrative for how the operator() is supposed to be set up
struct DoNothing : BaseAlgorithm {
    void operator()(Ferry& ferry, Yard& yard, FileHandler& fh, std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel = {}) override {};
    bool operator()(Yard& yard, Vehicle& vehicle) override { return false; }
    virtual std::string AlgorithmName() { return "Do nothing"; }
};

// Algorithm that does nothing, just for testing how the operator() overloading works
// Also illustrative for how the operator() is supposed to be set up
struct WorstFitParallel : BaseAlgorithm {
    WorstFit worstfit;
    void operator()(Ferry& ferry, Yard& yard, FileHandler& fh, std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel = {}) override;
    bool operator()(Yard& yard, Vehicle& vehicle) override;
    virtual std::string AlgorithmName() { return "Parallel worst fit"; }
};


#endif //EIT_ALGORITHMS_H
