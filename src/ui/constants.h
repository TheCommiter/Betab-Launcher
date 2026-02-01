#pragma once

#include <SDL.h>

namespace vita::ui {
constexpr int kBaseWidth = 960;
constexpr int kBaseHeight = 544;
constexpr int kMaxPages = 10;
constexpr int kMaxIcons = 500;

constexpr int kInfoBarHeight = 44;
constexpr int kPageDotY = kBaseHeight - 24;
constexpr int kPageDotRadius = 4;
constexpr int kPageDotSpacing = 14;

constexpr int kIconSize = 96;
constexpr int kIconPaddingX = 24;
constexpr int kIconPaddingY = 20;
constexpr int kGridColumns = 5;
constexpr int kGridRows = 3;
constexpr int kGridTop = 80;
constexpr int kGridLeft = 60;

constexpr int kHeroWidth = 720;
constexpr int kHeroHeight = 405;

constexpr int kGateButtonWidth = 240;
constexpr int kGateButtonHeight = 64;

constexpr int kFocusScaleDurationMs = 120;
constexpr int kPageTransitionMs = 260;
constexpr int kNotificationToastMs = 2800;
constexpr int kQuickMenuFadeMs = 200;

constexpr SDL_Color kColorBackground{11, 13, 18, 255};
constexpr SDL_Color kColorScrim{0, 0, 0, 166};
constexpr SDL_Color kColorPanel{21, 26, 36, 235};
constexpr SDL_Color kColorTextPrimary{255, 255, 255, 255};
constexpr SDL_Color kColorTextSecondary{183, 189, 201, 255};
constexpr SDL_Color kColorFocus{255, 255, 255, 255};
constexpr SDL_Color kColorDotInactive{120, 130, 146, 255};
constexpr SDL_Color kColorDotActive{255, 255, 255, 255};
}  // namespace vita::ui
