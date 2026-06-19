#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <imgui-cocos.hpp>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace geode::prelude;

// Data structures
struct QueueEntry {
    std::string title;
    std::string requester;
    std::string creator;
    bool expanded;
    
    QueueEntry(const std::string& t = "", const std::string& r = "", const std::string& c = "")
        : title(t), requester(r), creator(c), expanded(false) {}
};

struct BannedUser {
    std::string name;
    bool isRequester; // true = requester, false = creator
};

class HwGDReqsApp {
public:
    HwGDReqsApp() {
        queueEntries.push_back(QueueEntry("Bloodbath", "riot", "acoolgamer"));
        queueEntries.push_back(QueueEntry("Tidal Wave", "onilink", "anotherperson"));
        
        for (int i = 0; i < 4; i++) {
            queueEntries.push_back(QueueEntry());
        }
        
        // sample banned users
        bannedUsers.push_back({"Requester_Alpha", true});
        bannedUsers.push_back({"Requester_Gamma", true});
        bannedUsers.push_back({"Requester_Omega", true});
        bannedUsers.push_back({"Creator_42", false});
        bannedUsers.push_back({"Creator_Blitz", false});
        bannedUsers.push_back({"Creator_Nyx", false});
        
        // default toggle states
        toggles["dontAllowPlayed"] = true;
        toggles["subsPriority"] = false;
        toggles["subsOnly"] = true;
        toggles["liveOnly"] = false;
        toggles["botReplies"] = true;
        toggles["enableRequests"] = true;
        
        // default tab
        currentTab = 0;
        showSettings = false;
    }
    
    void Render() {
        // Main window always visible
        RenderMainWindow();
        
        // Settings window (toggleable)
        if (showSettings) {
            RenderSettingsWindow();
        }
    }
    
private:
    std::vector<QueueEntry> queueEntries;
    std::vector<BannedUser> bannedUsers;
    std::map<std::string, bool> toggles;
    int currentTab;
    bool showSettings;
    
    void RenderMainWindow() {
        ImGui::SetNextWindowSize(ImVec2(440, 600), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
        
        ImGui::Begin("HwGDReqs by MalikHw47", nullptr, 
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        
        // Title
        ImGui::TextColored(ImVec4(0.94f, 0.94f, 0.98f, 1.0f), "HwGDReqs");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.69f, 1.0f), "by MalikHw47");
        ImGui::Separator();
        ImGui::Spacing();
        
        // Queue list
        ImGui::BeginChild("QueueList", ImVec2(0, 380), true);
        
        for (auto& entry : queueEntries) {
            if (entry.title.empty()) {
                ImGui::TextColored(ImVec4(0.43f, 0.43f, 0.54f, 0.7f), "— empty slot —");
                continue;
            }
            
            // Main line with title and expand button
            ImGui::PushID(&entry);
            
            ImGui::TextColored(ImVec4(0.92f, 0.92f, 0.96f, 1.0f), "%s", entry.title.c_str());
            ImGui::SameLine(ImGui::GetWindowWidth() - 40);
            
            std::string expandLabel = entry.expanded ? "▼" : "▶";
            if (ImGui::SmallButton(expandLabel.c_str())) {
                entry.expanded = !entry.expanded;
            }
            
            // Subtitle
            std::string subtitle = entry.requester + " - " + entry.creator;
            ImGui::TextColored(ImVec4(0.69f, 0.69f, 0.8f, 1.0f), "%s", subtitle.c_str());
            
            // Expandable actions
            if (entry.expanded) {
                ImGui::Separator();
                ImGui::Spacing();
                
                if (ImGui::Button("Delete", ImVec2(60, 0))) {
                    // Delete entry
                    entry.title = "";
                    entry.requester = "";
                    entry.creator = "";
                }
                ImGui::SameLine();
                
                if (ImGui::Button("Ban Requester", ImVec2(100, 0))) {
                    // Ban requester
                    if (!entry.requester.empty()) {
                        bannedUsers.push_back({entry.requester, true});
                    }
                }
                ImGui::SameLine();
                
                if (ImGui::Button("Ban Creator", ImVec2(90, 0))) {
                    // Ban creator
                    if (!entry.creator.empty()) {
                        bannedUsers.push_back({entry.creator, false});
                    }
                }
                
                ImGui::Spacing();
            }
            
            ImGui::PopID();
            ImGui::Separator();
        }
        
        ImGui::EndChild();
        ImGui::Spacing();
        
        // Controls row
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.23f, 1.0f));
        ImGui::BeginChild("Controls", ImVec2(0, 60), true);
        
        // Enable requests toggle
        bool* enableRequests = &toggles["enableRequests"];
        ImGui::Text("enable requests");
        ImGui::SameLine();
        ImGui::Checkbox("##EnableRequests", enableRequests);
        
        ImGui::SameLine(ImGui::GetWindowWidth() - 220);
        
        // Buttons
        if (ImGui::Button("random", ImVec2(60, 0))) {
            // Random button action
        }
        ImGui::SameLine();
        
        if (ImGui::Button("settings", ImVec2(60, 0))) {
            showSettings = !showSettings;
        }
        ImGui::SameLine();
        
        if (ImGui::Button("Clear queue", ImVec2(80, 0))) {
            // Clear all non-empty entries
            for (auto& entry : queueEntries) {
                if (!entry.title.empty()) {
                    entry.title = "";
                    entry.requester = "";
                    entry.creator = "";
                }
            }
        }
        
        ImGui::EndChild();
        ImGui::PopStyleColor();
        
        ImGui::End();
    }
    
    void RenderSettingsWindow() {
        ImGui::SetNextWindowSize(ImVec2(440, 500), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(560, 100), ImGuiCond_FirstUseEver);
        
        ImGui::Begin("HwGDReqs Settings", &showSettings,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        
        // Title
        ImGui::TextColored(ImVec4(0.94f, 0.94f, 0.98f, 1.0f), "HwGDReqs");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.69f, 1.0f), "Settings");
        ImGui::Separator();
        ImGui::Spacing();
        
        if (ImGui::Button("login", ImVec2(60, 0))) {
            // action
        }
        ImGui::SameLine();
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.45f);
        ImGui::Button("logout", ImVec2(60, 0));
        ImGui::PopStyleVar();
        ImGui::Spacing();
        
        RenderToggle("don't allow already played levels", "dontAllowPlayed");
        RenderToggle("Subs priority (twitch)", "subsPriority");
        RenderToggle("subs only (twitch)", "subsOnly");
        RenderToggle("live only (twitch)", "liveOnly");
        RenderToggle("bot replies (twitch)", "botReplies");
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        // Tabs
        if (ImGui::Button("banned requesters", ImVec2(150, 0))) {
            currentTab = 0;
        }
        ImGui::SameLine();
        if (ImGui::Button("banned creators", ImVec2(150, 0))) {
            currentTab = 1;
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        
        // Tab content
        ImGui::BeginChild("BannedList", ImVec2(0, 180), true);
        
        if (currentTab == 0) {
            // Show banned requesters
            for (const auto& user : bannedUsers) {
                if (user.isRequester) {
                    ImGui::Text("🔸 %s", user.name.c_str());
                    ImGui::SameLine(ImGui::GetWindowWidth() - 70);
                    if (ImGui::SmallButton("unban")) {
                        // Unban action
                    }
                }
            }
            size_t requesterCount = std::count_if(bannedUsers.begin(), bannedUsers.end(), 
                [](const BannedUser& u) { return u.isRequester; });
            ImGui::TextColored(ImVec4(0.43f, 0.43f, 0.54f, 1.0f), 
                "(%zu banned requesters)", requesterCount);
        } else {
            // Show banned creators
            for (const auto& user : bannedUsers) {
                if (!user.isRequester) {
                    ImGui::Text("🎮 %s", user.name.c_str());
                    ImGui::SameLine(ImGui::GetWindowWidth() - 70);
                    if (ImGui::SmallButton("unban")) {
                        // Unban action
                    }
                }
            }
            size_t creatorCount = std::count_if(bannedUsers.begin(), bannedUsers.end(), 
                [](const BannedUser& u) { return !u.isRequester; });
            ImGui::TextColored(ImVec4(0.43f, 0.43f, 0.54f, 1.0f), 
                "(%zu banned creators)", creatorCount);
        }
        
        ImGui::EndChild();
        
        ImGui::End();
    }
    
    void RenderToggle(const char* label, const std::string& key) {
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::Checkbox(("##" + key).c_str(), &toggles[key]);
    }
};

// Global app instance and visibility state
static HwGDReqsApp* g_app = nullptr;
static bool g_showUI = false;

$on_mod(Loaded) {
    // Initialize the app
    g_app = new HwGDReqsApp();
    
    // Setup ImGuiCocos with the app
    ImGuiCocos::get().setup([] {
        // Setup fonts and styling
        ImGuiIO& io = ImGui::GetIO();
        
        // Load Arial font from resources
        auto fontPath = Mod::get()->getResourcesDir() / "arial.ttf";
        ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 16.0f);
        if (font) {
            io.FontDefault = font;
        }
        
        // Setup style
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        
        // Window rounding
        style.WindowRounding = 8.0f;
        style.ChildRounding = 0.0f;
        style.FrameRounding = 0.0f;
        style.PopupRounding = 0.0f;
        style.ScrollbarRounding = 0.0f;
        style.GrabRounding = 0.0f;
        style.TabRounding = 0.0f;
        
        // Custom dark theme
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.25f, 1.0f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.13f, 0.20f, 1.0f);
        colors[ImGuiCol_Border] = ImVec4(0.29f, 0.29f, 0.38f, 1.0f);
        colors[ImGuiCol_Text] = ImVec4(0.91f, 0.91f, 0.94f, 1.0f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.43f, 0.43f, 0.54f, 1.0f);
        colors[ImGuiCol_Button] = ImVec4(0.23f, 0.23f, 0.34f, 1.0f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.31f, 0.31f, 0.44f, 1.0f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.16f, 0.16f, 0.26f, 1.0f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.42f, 0.55f, 1.0f, 1.0f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.42f, 0.55f, 1.0f, 1.0f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.42f, 0.55f, 1.0f, 1.0f);
        colors[ImGuiCol_Separator] = ImVec4(0.29f, 0.29f, 0.38f, 1.0f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.13f, 0.20f, 1.0f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.27f, 1.0f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.23f, 0.23f, 0.34f, 1.0f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.18f, 0.18f, 0.25f, 1.0f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.18f, 0.25f, 1.0f);
    }).draw([] {
        if (g_showUI && g_app) {
            g_app->Render();
        }
    });
}

// KeyDown hook to toggle with 'T' key
class $modify(CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool repeat) {
        if (down && !repeat && key == KEY_T) {
            g_showUI = !g_showUI;
        }
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);
    }
};
