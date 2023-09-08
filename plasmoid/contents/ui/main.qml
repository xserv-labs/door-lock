import QtQuick 2.0
import QtQuick.Layouts 1.0
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.components 3.0 as PlasmaComponents3



Item {
    id: widget

    // Always display the compact view.
    // Never show the full popup view even if there is space for it.
    Plasmoid.preferredRepresentation: Plasmoid.compactRepresentation

    Plasmoid.fullRepresentation: Item {
        Layout.minimumWidth: 100
        Layout.minimumHeight: 100
        Layout.preferredWidth: Layout.minimumWidth
        Layout.preferredHeight: Layout.minimumHeight
        Layout.maximumWidth: 200
        Layout.maximumHeight: 200

        PlasmaComponents3.ToolButton {
            icon.name: "unlock-symbolic"
            text: i18n("unlock")
            anchors.centerIn: parent
            onClicked: {
                var xhr = new XMLHttpRequest();
                xhr.onreadystatechange = function () {
                    if (xhr.readyState === XMLHttpRequest.DONE) {
                        if (xhr.status !== 200) {
                            console.error("API Request Failed:", xhr.status, xhr.statusText);
                        }
                    }
                };
                xhr.open(
                    "GET",
                    "https://door.xserv.dev/unlock/cm?cmnd=Backlog%20Power%20off%3B%20Delay%0220%3B%20Power%20on%3B%20Delay%0220%3B%20Power%20off",
                    true
                );
                xhr.send();
            }
        }
    }
}
