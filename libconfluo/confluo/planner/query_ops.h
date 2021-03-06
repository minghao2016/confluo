#ifndef CONFLUO_PLANNER_QUERY_OPS_H_
#define CONFLUO_PLANNER_QUERY_OPS_H_

#include "container/data_log.h"
#include "container/lazy/stream.h"
#include "container/radix_tree.h"
#include "container/record_offset_range.h"
#include "schema/schema.h"
#include "container/cursor/offset_cursors.h"

namespace confluo {
namespace planner {

/**
 * Type of operation that is executed
 */
enum query_op_type {
  /** Operation that does nothing */
      D_NO_OP = 0,
  /** Operation that scans the data store */
      D_SCAN_OP = 1,
  /** Operation that is invalid */
      D_NO_VALID_INDEX_OP = 2,
  /** Index operation */
      D_INDEX_OP = 3
};

/**
 * Query operation class. Contains functionality for all queries.
 */
class query_op {
 public:
  /**
   * Constructs a query operation from a type of query op
   *
   * @param op The type of query operation
   */
  query_op(const query_op_type &op);

  /**
   * Destructs the query operation
   */
  virtual ~query_op();

  /**
   * Gets the operation type
   *
   * @return The query operation type
   */
  query_op_type op_type();

  /**
   * Gets the cost of the query operation
   *
   * @return The query operation cost
   */
  virtual uint64_t cost() const = 0;

  /**
   * Gets a string representation of the query operation
   *
   * @return The string representation of the query operation
   */
  virtual std::string to_string() const = 0;

 private:
  query_op_type op_;
};

/**
 * The no operation class. Specific implementation of a query operation
 * that does nothing
 */
class no_op : public query_op {
 public:
  /**
   * Constructs a no op query operation
   */
  no_op();

  /**
   * Gets a string representation of the no op
   *
   * @return A string containing the representation of no op
   */
  virtual std::string to_string() const override;

  /**
   * Gets the cost of a no op
   *
   * @return The cost of a no op
   */
  virtual uint64_t cost() const override;
};

/**
 * No valid index operation
 */
class no_valid_index_op : public query_op {
 public:
  /**
   * Constructs a no valid index query operation
   */
  no_valid_index_op();

  /**
   * Gets a string representation of the no valid index operation
   *
   * @return A string containing the representation of a no valid index
   * operation
   */
  virtual std::string to_string() const override;

  /**
   * Gets the cost of the no valid index operation
   *
   * @return The cost of the no valid index operation
   */
  virtual uint64_t cost() const override;
};

/**
 * Full scan operation class. Specific implementation of a query operation
 * that does a full scan.
 */
class full_scan_op : public query_op {
 public:
  full_scan_op();

  /**
   * Gets a string representation of the full scan operation
   *
   * @return A formatted string containing the name and expression
   * of the operation
   */
  virtual std::string to_string() const override;

  /**
   * Gets the cost of the full scan operation
   *
   * @return The cost of the full scan operation
   */
  virtual uint64_t cost() const override;
};

/**
 * Index operation class. A specific implementation of a query operation
 * that indexes the data store.
 */
class index_op : public query_op {
 public:
  /** The key range for the index operation */
  typedef std::pair<byte_string, byte_string> key_range;

  /**
   * Initializes the index operation
   *
   * @param index The radix index
   * @param range The key range for the index
   */
  index_op(const index::radix_index *index, const key_range &range);

  /**
   * Gets a string representation of the index operation
   *
   * @return Information about the index operation in a string
   */
  virtual std::string to_string() const override;

  /**
   * Gets the cost of the index operation
   *
   * @return The cost of the index operation
   */
  virtual uint64_t cost() const override;

  /**
   * The query index operation
   *
   * @return The radix index for the query index
   */
  index::radix_index::rt_result query_index();

 private:
  const index::radix_index *index_;
  key_range range_;
};

}
}

#endif /* CONFLUO_PLANNER_QUERY_OPS_H_ */
