#ifndef DOXYGENINFO_H
#define DOXYGENINFO_H

/**
 * @file
 * @brief Front page of ScriptEmbedder doxygen-documentation.
 * @author Perttu Paarlahti 2016.
 */

/**
 * @mainpage The ScriptEmbedder component documentation.
 *
 * @section General
 *
 *  The ScriptEmbedder component is a generalized version of the SignalHandler application used in the Auxilo2 smart home project
 *  (http://github.com/coder4803/auxilo2). SignalHandler was a stand alone application that was responsible for executing certain scripts
 *  in response to messages received from the system. Being a complete application restricts its reuability in some other context. ScriptEmbedder is
 *  designed to be a reusable component, that is not dependent on Auxilo2 components, interfaces or architecture, and therefore can be reused
 *  in totally different context. The SignalHandler application may be later refactored to use this component to ease maintenance and deploy extra
 *  features added into ScriptEmbedder.
 *
 *  Source code for the software is published under MIT-licence. See the LICENCE-file in the git repository root for detailed conditions.
 *
 *
 * @section Features
 *
 *  General purpose: The ScriptEmbedder component allows user to embed scripts into their C++ application and provide a call-back interface that
 *  scripts can use to interact with the C++ host application. Embedding scripts increases application's run-time flexibility: Application's
 *  functionality can be altered simply modifying script files. no changes to C++ code re-compilation is required. Changes can be applied even at
 *  run-time. There are no restrictions on supported languages - scripting language support is implemented as plugins, which makes it possible to
 *  add support for any language. Central features of this component are:
 *
 *  1) Defining a set of accepted scripts and supported scripting languages.
 *
 *  2) Changeing scripts and supported languages at run-time.
 *
 *  3) Running scripts.
 *
 *  4) Allowing scripts to interact with the host application through user-defined API.
 *
 *  5) Reporting script run results through user-provided interface.
 *
 *  6) Provides interfaces for adding new interpreters. See GitHub wiki page for more detailed description.
 *
 *
 * @section Contents
 *
 *  This document describes public interfaces and classes in the ScriptEmbedder component. Public class documentation consists of description of
 *  all available methods, and how to use them appropriately (pre- and post conditions). Interface implementations or other internal details are
 *  not included, even though these classes are documented in the code in a similiar way using doxygen. Test projects and examples are not
 *  included eather. You may find information about those in GitHub project's wiki page.
 *
 *  Start exploring the document using the top bar menu.
 */

#endif // DOXYGENINFO_H

