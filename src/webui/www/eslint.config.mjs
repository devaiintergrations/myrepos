import Globals from "globals";
import Html from "eslint-plugin-html";
import Js from "@eslint/js";
import Stylistic from "@stylistic/eslint-plugin";
import * as RegexpPlugin from "eslint-plugin-regexp";

export default [
    Js.configs.recommended,
    RegexpPlugin.configs["flat/recommended"],
    Stylistic.configs["disable-legacy"],
    {
        files: [
            "**/*.html",
            "**/*.js",
            "**/*.mjs"
        ],
        languageOptions: {
            ecmaVersion: 2022,
            globals: {
                ...Globals.browser
            }
        },
        plugins: {
            Html,
            RegexpPlugin,
            Stylistic
        },
        rules: {
            "eqeqeq": "error",
            "guard-for-in": "error",
            "no-undef": "off",
            "no-unused-vars": "off",
            "no-var": "error",
            "operator-assignment": "error",
            "prefer-arrow-callback": "error",
            "prefer-const": "error",
            "radix": "error",
            "Stylistic/no-mixed-operators": [
                "error",
                {
                    "groups": [
                        ["&", "|", "^", "~", "<<", ">>", ">>>", "==", "!=", "===", "!==", ">", ">=", "<", "<=", "&&", "||", "in", "instanceof"]
                    ]
                }
            ],
            "Stylistic/nonblock-statement-body-position": ["error", "below"],
            "Stylistic/quotes": [
                "error",
                "double",
                {
                    "avoidEscape": true,
                    "allowTemplateLiterals": true
                }
            ],
            "Stylistic/semi": "error"
        }
    }
];
