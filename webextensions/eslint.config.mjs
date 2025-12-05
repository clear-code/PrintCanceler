import js from "@eslint/js";
import globals from "globals";
import { defineConfig } from "eslint/config";


export default defineConfig([
  { 
    files: ["**/*.{js,mjs,cjs}"],
    plugins: { js },
    extends: ["js/recommended"],
  },
  { 
    files: ["**/*.{js,mjs,cjs}"],
    ignores: ["!**/.eslintrc.js", "**/eslint.config.mjs"],
    languageOptions: {
      globals: { 
        ...globals.browser,
        chrome: "readonly",
        module: "readonly",
        exports: "readonly",
      },
    },
    rules: {
      "no-const-assign": "error",

      "prefer-const": ["warn", {
          destructuring: "any",
          ignoreReadBeforeAssign: false,
      }],

      "no-var": "error",

      "no-unused-vars": ["warn", {
          vars: "all",
          args: "after-used",
          argsIgnorePattern: "^_",
          caughtErrors: "all",
          caughtErrorsIgnorePattern: "^_",
      }],

      "no-use-before-define": ["error", {
          functions: false,
          classes: true,
      }],

      "no-unused-expressions": "error",
      "no-unused-labels": "error",

      "no-undef": ["error", {
          typeof: true,
      }],

      indent: ["warn", 2, {
          SwitchCase: 1,
          MemberExpression: 1,

          CallExpression: {
              arguments: "first",
          },

          VariableDeclarator: {
              var: 2,
              let: 2,
              const: 3,
          },
      }],

      "no-underscore-dangle": ["warn", {
          allowAfterThis: true,
      }],

      quotes: ["warn", "single", {
          avoidEscape: true,
          allowTemplateLiterals: true,
      }],
    },
  },
]);
