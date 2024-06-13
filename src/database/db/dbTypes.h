#ifndef DB_TYPES_H
#define DB_TYPES_H

#include <string>
#include <unordered_map>

namespace db
{

/* LEF LAYER */
enum RoutingType    {ROUTING, CUT, MASTERSLICE, OVERLAP};

// MASTERSLICE layers are typically polysilicon layers 
// and are only needed if the cell MACROs have pins on the polysilicon layer.

// OVERLAP layer can be used in MACRO definitions to form rectilinear-shaped cells (and blocks)
// e.g. L-shaped block

enum LayerDirection {HORIZONTAL, VERTICAL};

/* LEF SITE */
enum SiteClass      {CORE_SITE};

/* LEF MACRO */
enum MacroClass     {CORE, CORE_FEEDTHRU, CORE_TIEHIGH, CORE_TIELOW, CORE_SPACER, CORE_WELLTAP, CORE_ANTENNACELL,
                     PAD, 
                     BLOCK, 
                     ENDCAP};

/* LEF PIN */
enum PinDirection   {INPUT, OUTPUT, INOUT};
enum PinUsage       {SIGNAL, POWER, GROUND, CLOCK};
enum PinShape       {ABUTMENT, RING, FEEDTHRU};

/* DEF COMPONENT */
enum Orient         {N, S, W, E, FN, FS, FW, FE};
enum Source         {DIST, NETLIST, TIMING, USER}; // SOURCE of a COMPONENT
// DIST    : Physical components like well-taps, filler cells, decoupling caps
// NETLIST : Components that are specified in original netlist
// TIMING  : Timing buffer / Clock tree (physical rather than logical)
// USER    : Components that are generated by user-defined reason

#undef Status
enum Status         {PLACED, FIXED, COVER, UNPLACED};

/* DEF NET */
enum NetUse         {ANALOG_NET, CLOCK_NET, GROUND_NET, POWER_NET, RESET_NET, SCAN_NET, SIGNAL_NET, TIEOFF_NET};

class dbTypes
{
  public:

    dbTypes(); // Initialized by default constructor

    RoutingType    getRoutingType    (const std::string& str) const;
    LayerDirection getLayerDirection (const std::string& str) const;
    MacroClass     getMacroClass     (const std::string& str) const;
    SiteClass      getSiteClass      (const std::string& str) const;
    PinDirection   getPinDirection   (const std::string& str) const;
    PinUsage       getPinUsage       (const std::string& str) const;
    PinShape       getPinShape       (const std::string& str) const;
    Orient         getOrient         (const std::string& str) const;
    Source         getSource         (const std::string& str) const;
    NetUse         getNetUse         (const std::string& str) const;
    Status         getStatus         (int status)             const;

  private:

    std::unordered_map<std::string, RoutingType>    str2RoutingType_;     // String - enum RoutingType    Table
    std::unordered_map<std::string, LayerDirection> str2LayerDirection_;  // String - enum LayerDirection Table
    std::unordered_map<std::string, MacroClass>     str2MacroClass_;      // String - enum MACRO_CLASS    Table
    std::unordered_map<std::string, SiteClass>      str2SiteClass_;       // String - enum SITE_CLASS     Table
    std::unordered_map<std::string, PinDirection>   str2PinDirection_;    // String - enum PIN_DIRECTION  Table
    std::unordered_map<std::string, PinUsage>       str2PinUsage_;        // String - enum PIN_USAGE      Table
    std::unordered_map<std::string, PinShape>       str2PinShape_;        // String - enum PIN_SHAPE      Table
    std::unordered_map<std::string, Orient>         str2Orient_;          // String - enum ORIENT         Table
    std::unordered_map<std::string, Source>         str2Source_;          // String - enum SOURCE         Table
    std::unordered_map<std::string, NetUse>         str2NetUse_;          // String - enum USE (def net)  Table
    std::unordered_map<int, Status>                 int2Status_;          // Int    - enum STATUS         Table
	  // defrReader API does not define Placement Status as const char* (string).
		// Instead, it gives us placement status as an integer.
};

}

#endif
