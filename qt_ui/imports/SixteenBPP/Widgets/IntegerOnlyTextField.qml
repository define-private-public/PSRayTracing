import QtQuick
import QtQuick.Controls

// This is a TextField that only allows for integer input
TextField {
  id: root

  property alias lower_bound: validator.bottom
  property alias upper_bound: validator.top

  validator: IntValidator {
    id: validator
  }

  // "No predicted" is required so we can get changed signals to fire continuously
  inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText

  // Will parse the text value to an integer
  function value_as_int() {
    return parseInt(root.text);
  }
}
