#include "Assets/Sprites.hpp"
#include "Assets/Gif.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/WebUtils.hpp"

#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/SpriteMeshType.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"

using UnityEngine::Sprite;

map<string, UnityEngine::Sprite*> Sprites::iconCache;

void Sprites::get_Icon(string url, std::function<void(UnityEngine::Sprite*)> completion) {
    if (iconCache.count(url)) {
        completion(iconCache[url]);
    } else {
        WebUtils::GetAsync(url, [completion, url](long code, string data) {
            if (code == 200) {
                QuestUI::MainThreadScheduler::Schedule([completion, data, url] {
                    std::vector<uint8_t> bytes(data.begin(), data.end());
                    Array<uint8_t>* spriteArray = il2cpp_utils::vectorToArray(bytes);
                    UnityEngine::Sprite* sprite;
                    Gif gif = Gif(spriteArray);
                    if (gif.Parse() == 0 && gif.Slurp() == 1) {
                        auto tex = gif.get_frame(0);
                        sprite = UnityEngine::Sprite::Create(
                            tex, 
                            UnityEngine::Rect(0.0, 0.0, gif.get_width(), gif.get_height()), 
                            UnityEngine::Vector2(0.5, 0.5), 
                            100.0, 
                            0, 
                            UnityEngine::SpriteMeshType::FullRect, 
                            UnityEngine::Vector4(0.0, 0.0, 0.0, 0.0), 
                            false);
                    } else {
                        sprite = QuestUI::BeatSaberUI::ArrayToSprite(spriteArray);
                    }

                    if (sprite != NULL) {
                        iconCache[url] = sprite;
                        completion(sprite);
                    }
                });
            }
        });
    }
}

void Sprites::GetCountryIcon(string country, std::function<void(UnityEngine::Sprite*)> completion) {
    string lowerCountry;
    lowerCountry.resize(country.size());
    transform(country.begin(), country.end(), lowerCountry.begin(), asciitolower);
    
    get_Icon("https://cdn.beatleader.xyz/flags/" + lowerCountry + ".png", completion);
}

const string Sprites::CellBGB64 = "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAABhGlDQ1BJQ0MgcHJvZmlsZQAAKJF9kTtIw0Acxr8+pD4qDnYQcchQBcGCqIijVqEIFUKt0KqDyaUvaNKQpLg4Cq4FBx+LVQcXZ10dXAVB8AHi6OSk6CIl/i8ptIjx4Lgf3933cfcd4K+XmWoGxwFVs4xUIi5ksqtC6BVBdAHowajETH1OFJPwHF/38PH1LsazvM/9OXqVnMkAn0A8y3TDIt4gnt60dM77xBFWlBTic+Ixgy5I/Mh12eU3zgWH/TwzYqRT88QRYqHQxnIbs6KhEk8RRxVVo3x/xmWF8xZntVxlzXvyF4Zz2soy12kOIYFFLEGEABlVlFCGhRitGikmUrQf9/APOn6RXDK5SmDkWEAFKiTHD/4Hv7s185MTblI4DnS82PbHMBDaBRo12/4+tu3GCRB4Bq60lr9SB2Y+Sa+1tOgR0LcNXFy3NHkPuNwBBp50yZAcKUDTn88D72f0TVmg/xboXnN7a+7j9AFIU1fJG+DgEBgpUPa6x7s723v790yzvx8RVnKA8zW+DAAAAAZiS0dEAAAAAAAA+UO7fwAAAAlwSFlzAAAuIwAALiMBeKU/dgAAAAd0SU1FB+YECQAUNxJSyS8AAAAZdEVYdENvbW1lbnQAQ3JlYXRlZCB3aXRoIEdJTVBXgQ4XAAAAJ0lEQVRYw+3QQREAAAjDMMC/5yGDB4mCXqsAAAC+6yS5DJjrAwIELCTqBAjIG8enAAAAAElFTkSuQmCC";

UnityEngine::Sprite* Sprites::get_CellBG() {
    return QuestUI::BeatSaberUI::Base64ToSprite(const_cast<string&>(CellBGB64));
}

const string Sprites::UpB64 = "iVBORw0KGgoAAAANSUhEUgAAAJYAAABfCAMAAAA9OM6sAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAhGVYSWZNTQAqAAAACAAFARIAAwAAAAEAAQAAARoABQAAAAEAAABKARsABQAAAAEAAABSASgAAwAAAAEAAgAAh2kABAAAAAEAAABaAAAAAAAAAGAAAAABAAAAYAAAAAEAA6ABAAMAAAABAAEAAKACAAQAAAABAAAAlqADAAQAAAABAAAAXwAAAADvKUNOAAAACXBIWXMAAA7EAAAOxAGVKw4bAAABWWlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iWE1QIENvcmUgNi4wLjAiPgogICA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPgogICAgICA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIgogICAgICAgICAgICB4bWxuczp0aWZmPSJodHRwOi8vbnMuYWRvYmUuY29tL3RpZmYvMS4wLyI+CiAgICAgICAgIDx0aWZmOk9yaWVudGF0aW9uPjE8L3RpZmY6T3JpZW50YXRpb24+CiAgICAgIDwvcmRmOkRlc2NyaXB0aW9uPgogICA8L3JkZjpSREY+CjwveDp4bXBtZXRhPgoZXuEHAAACiFBMVEUAAAAAAACAgIBVVVVAQEAzMzNmZmZVVVVJSUlAQEBgYGBVVVVNTU1GRkZVVVVOTk5JSUlVVVVQUFBLS0tVVVVRUVFNTU1RUVFOTk5VVVVOTk5SUlJPT09SUlJNTU1VVVVTU1NQUFBOTk5TU1NRUVFOTk5TU1NRUVFPT09TU1NRUVFPT09OTk5TU1NRUVFQUFBOTk5TU1NSUlJQUFBTU1NSUlJQUFBPT09TU1NSUlJRUVFPT09SUlJRUVFPT09SUlJRUVFQUFBSUlJRUVFQUFBPT09RUVFQUFBPT09TU1NRUVFQUFBPT09TU1NSUlJRUVFPT09SUlJRUVFSUlJRUVFSUlJQUFBQUFBRUVFQUFBRUVFRUVFQ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";

UnityEngine::Sprite* Sprites::get_UpIcon() {
    return QuestUI::BeatSaberUI::Base64ToSprite(const_cast<string&>(UpB64));
}

const string Sprites::DownB64 = "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";

UnityEngine::Sprite* Sprites::get_DownIcon() {
    return QuestUI::BeatSaberUI::Base64ToSprite(const_cast<string&>(DownB64));
}

const string Sprites::BeatLeaderB64 = "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";

Sprite* Sprites::get_BeatLeaderIcon() {
    return QuestUI::BeatSaberUI::Base64ToSprite(const_cast<string&>(BeatLeaderB64));
}

const string Sprites::StarB64 = "iVBORw0KGgoAAAANSUhEUgAAAIAAAACACAMAAAD04JH5AAAB5lBMVEX////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ZkfksAAAAoXRSTlMAAQIDBAUGCAkKCwwNEBESExUWFxgZGxweHyQlJicrLC0vMTQ2Nzg5Oj9AQUJFSElKS0xOUlNUVldYWlxeX2BhYmRlaGtsbm9xcnR1dnd6e35/hYeIiouOk5SWmJmcn6Gio6Slp6ipra6xsrO1t7u8vb6/wcLDxMXGx8jJysvO0NHW19jZ2tvd3t/i5OXm6Ovt7/Dx8vP09fb3+Pn6+/z9/nFfuYEAAAQpSURBVHgB7dkJN1RhAwfw/8VQSCVFjaRQoSQtKbQv0hJlSdGuRKSlSEUhRUiNlAzm/03f88qZY5Z7Z5773Pvc9z2n39f44Z//byk3R0dvpsAxrmck+cwFp1RyUSUcEjvCRSOxcMZ+LtkPZ7zikldwRA79cuCE+/S7Dwds8NLPuwHqXeMy16DcykkuM7kSqp1igFNQTBtkgEENahUzSDHU6mSQTii1hSG2QKVmhmiGQmtnGGJmLdSpZhjVUCZ+gmFMxEOVCoZVAVX6GVY/FCmkjkKo0UYdbVDC7aMOnxsqXKeu61Bg1TR1Ta+C/S7QwAXYLm6UBkbjYLdSGiqF3XppqBc2284ItsNeDxnBQ9gqY54RzGfATvWMqB42SvrOiL4nwT5nGIUzsE3MMKMwHAO77GNU9sEWcem53YxKd256HKyipW7be7Kmqb1vYoECFib62ptqTu7dlqrBlJSs3RWXG1t7R72U5B3tbW28XLE7KwWRJboLjlQ1tLz8/Js2+P35ZUtD1ZECdyKCZOw8eLb2bteAh4p4Brru1p49uDMD/1X+gY75UA4U+uggXyHe0FFvMERHDaGLjupC9hwdNJcNHJ6lY2YPA8AuDx3i2YVFW8fpiPGtWLJxgA4Y2Ai/NT1UrmcNllnxmIo9XoEAsU1UqikWwaqpUDXCODFPReZPIKySX1TiVwl05H2jAt/yoCvzC233JRMG0vpos740GErupK06kxGB6x5tdM+FiLQ62qZOQzTO+WgL3zlE6dAf2uDPIUSt4Act96MAArK/0mJfsyEk4yMt9TEDglJe00KvUyAsoZWWaU2ACTE3aJEbMTDnCi1xBaYdm6O0uWOQsGeakqb3QEoxJRVDTikllUJOAyU1QE43JXVDimuGkmZckJFDaTmQcZrSTkPGHUq7AxmfKO0TJKymBVbDvCJaoAjm1dACNTCvgxbogHmTtMAkTHPTEm6YVUZLlMGsRlqiEWa9pyXew6QELy3hTYA5O2iRHTCnkhaphDkttEgLzBmhRUZgyjpGwetlFNbZlvUdmzd32Bb6VxnJcAkAlAwzkqsw4ymN/bwUj0Xxl37S2FOYEOOhEd/t9fBbf9tHI54YiMuikXd5CJD3jkayIO449U0c1RBEOzpBfcchrol6vPXJCCO53ks9TRDXTx1PMqEj8wl19ENY4jzDGiqGgeIhhjWfCFH5DGeqKh6G4qumGE4+RF1kKN+tNESUdsvHUBch6hFD9OYiKrm9DPEIosYYZLxcQ5S08nEGGYOgdAaarU2GgOTaWQZKh5gDDNC+CYI2tTPAAYip4zKDRTChaJDL1EHMc/p5zrtgiuu8h37PIeYFlyw0p8K01OYFLnkBMQ/4V08OpOT08K8HEOOeIsmxMg2StLIxkpxyQ1B+2/Tb6iRYIKn67XRbPv5X/fMfwXx1itpIs6EAAAAASUVORK5CYII=";

Sprite* Sprites::get_StarIcon() {
    return QuestUI::BeatSaberUI::Base64ToSprite(const_cast<string&>(StarB64));
}

const string Sprites::GraphB64 = "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";

Sprite* Sprites::get_GraphIcon() {
    return QuestUI::BeatSaberUI::Base64ToSprite(const_cast<string&>(GraphB64));
}

