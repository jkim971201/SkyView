#ifndef DB_TERM
#define DB_TERM

namespace db
{

class dbNet;

class dbTerm
{
  public:

    dbTerm();

    void print() const;

    // Setters
		void setPin  (dbPin*   pin) { pin_  = pin;  }
		void setNet  (dbNet*   net) { net_  = net;  }
		void setInst (dbInst* inst) { inst_ = inst; }

		// Getters
    const dbPin*   pin() const { return pin_;  }
    const dbNet*   net() const { return net_;  }
    const dbInst* inst() const { return inst_; }

  private:

		dbPin*  pin_;
		dbNet*  net_;
		dbInst* inst_;
};

}

#endif
