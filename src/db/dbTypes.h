#pragma once

#include <string>
#include <unordered_map>

namespace db
{

enum RoutingType    {ROUTING, CUT};
enum LayerDirection {HORIZONTAL, VERTICAL};
enum MacroClass     {CORE, CORE_SPACER, PAD, BLOCK, ENDCAP};
enum SiteClass      {CORE_SITE};
enum PinDirection   {INPUT, OUTPUT, INOUT};
enum PinUsage       {SIGNAL, POWER, GROUND, CLOCK};
enum Orient         {N, S, E, FN, FS};  // W E FW FE is not supported
enum Source         {DIST, NETLIST, TIMING, USER}; // SOURCE of a COMPONENT
// DIST    : Physical components like well-taps, filler cells, decoupling caps
// NETLIST : Components that are specified in original netlist
// TIMING  : Timing buffer / Clock tree (physical rather than logical)
// USER    : Components that are generated by user-defined reason

enum NetUsage     {ANALOG_NET, CLOCK_NET, GROUND_NET, POWER_NET, RESET_NET, SCAN_NET, SIGNAL_NET, TIEOFF_NET};

class dbTypes
{

  public:

    dbTypes(); // Initializey by default

		RoutingType    getRoutingType    (const std::string& str) const;
		LayerDirection getLayerDirection (const std::string& str) const;
		MacroClass     getMacroClass     (const std::string& str) const;
		SiteClass      getSiteClass      (const std::string& str) const;
		PinDirection   getPinDrection    (const std::string& str) const;
		PinUsage       getPinUsage       (const std::string& str) const;
		Orient         getOrient         (const std::string& str) const;
		Source         getSource         (const std::string& str) const;

	private:

    std::unordered_map<std::string, RoutingType>    strToRoutingType_;     // String - enum RoutingType    Table
    std::unordered_map<std::string, LayerDirection> strToLayerDirection_;  // String - enum LayerDirection Table
    std::unordered_map<std::string, MacroClass>     strToMacroClass_;      // String - enum MACRO_CLASS    Table
    std::unordered_map<std::string, SiteClass>      strToSiteClass_;       // String - enum SITE_CLASS     Table
    std::unordered_map<std::string, PinDirection>   strToPinDirection_;    // String - enum PIN_DIRECTION  Table
    std::unordered_map<std::string, PinUsage>       strToPinUsage_;        // String - enum PIN_USAGE      Table
    std::unordered_map<std::string, Orient>         strToOrient_;          // String - enum ORIENT         Table
    std::unordered_map<std::string, Source>         strToSource_;          // String - enum SOURCE         Table
};

}
