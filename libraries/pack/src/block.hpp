namespace koinos { namespace types { namespace protocol {

enum class header_hash_index : uint32
{
   /**
    * Hash of the previous block.
    */
   previous_block_hash_index = 0,

   /**
    * Hash of Merkle root of transactions.
    */
   transaction_merkle_root_hash_index = 1,

   /**
    * Hash of Merkle root of passive data.
    * Includes transaction passives, transaction signatures and block passives.
    */
   passive_data_merkle_root_hash_index = 2,

   /**
    * Number of header hashes.
    */
   NUM_HEADER_HASHES = 3
};

struct active_block_data
{
   /**
    * Hashes included in the header.
    * All hashes must use the same algorithm.
    */
   multihash_vector               header_hashes;

   /**
    * Block height.  The genesis block has height=1.
    */
   block_height_type              height;

   /**
    * The timestamp.  Must be zero.
    */
   timestamp_type                 timestamp;

   /**
    * Vector of opaque transactions in the block.
    */
   std::vector< variable_blob >   transactions;

   /**
    * A zero byte at the end, reserved for protocol expansion.
    */
   unused_extensions_type         extensions;
};

struct passive_block_data {};

struct passive_transaction_data {};

struct sig_block_data
{
   signature_type                 block_signature;
};

struct sig_transaction_data
{
   signature_type                 transaction_signature;
};

struct block_header
{
   /**
    * Block data that can read and write state
    * Deserializes as active_block_data
    */
   variable_blob                  active_bytes;

   /**
    * Block data that can only read state
    *
    * Element 0     :     passive_block_data
    * Element 1     :     sig_block_data
    *
    * Element 2*i+2 :     passive_transaction_data for i-th transaction
    * Element 2*i+3 :     sig_transaction_data     for i-th transaction
    */
   multihash                      passive_sig_merkle_root;
};

struct reserved_operation
{
   unused_extensions_type         extensions;
};

struct nop_operation
{
   unused_extensions_type         extensions;
};

struct create_system_contract_operation
{
   contract_id_type               contract_id;
   variable_blob                  bytecode;
   unused_extensions_type         extensions;
};

struct set_system_call_operation
{
   uint32                         call_id;
   system::system_call_target     target;
   unused_extensions_type         extensions;
};

struct contract_call_operation
{
   contract_id_type               contract_id;
   uint32                         entry_point;
   variable_blob                  args;
   unused_extensions_type         extensions;
};

typedef std::variant<
   reserved_operation,
   nop_operation,
   create_system_contract_operation,
   contract_call_operation,
   set_system_call_operation
   > operation;

struct reserved_block_header
{
   unused_extensions_type         extensions;
};

struct transaction_type
{
   /**
    * Vector of opaque operations in this transation
    */
   std::vector< variable_blob >   operations;

   /**
    * A zero byte at the end, reserved for protocol expansion.
    */
   unused_extensions_type         extensions;
};

struct reserved_segwit
{
   unused_extensions_type         extensions;
};

struct block_ref_segwit
{
   multihash                      ref_block_id;
   block_height_type              ref_block_height;
   unused_extensions_type         extensions;
};

struct expiration_segwit
{
   timestamp_type                 expiration_timestamp;
   unused_extensions_type         extensions;
};

struct signatures_segwit
{
   std::vector<signature_type>    signatures;
   unused_extensions_type         extensions;
};

struct contract_source_segwit
{
   multihash                      sourcehash;
   unused_extensions_type         extensions;
};

struct custom_segwit
{
   multihash                      world;
   variable_blob                  custom_data;
   unused_extensions_type         extensions;
};

typedef std::variant<
   reserved_segwit,
   block_ref_segwit,
   expiration_segwit,
   signatures_segwit,
   contract_source_segwit,
   custom_segwit > segwit_type;

} } } // koinos::types::protocol
