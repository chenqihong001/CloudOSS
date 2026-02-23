pragma Singleton
import QtQuick

QtObject {
    signal showRequested(string message, string type)
    function show(msg, type) {
        showRequested(msg, type || "info");
    }
    function error(msg) {
        showRequested(msg, "error");
    }
    function success(msg) {
        showRequested(msg, "success");
    }
    function warning(msg) {
        showRequested(msg, "warning");
    }
    function info(msg) {
        showRequested(msg, "info");
    }
}
