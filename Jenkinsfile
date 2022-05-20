#!/usr/bin/env groovy

pipeline {

  agent none

  environment {
    DOCKER_ARGS = ''
  }

  options {
    buildDiscarder logRotator(artifactNumToKeepStr: '3', artifactDaysToKeepStr: '5', daysToKeepStr: '4', numToKeepStr: '10')
    timeout(time: 150, unit: 'MINUTES')
    retry(1)
    parallelsAlwaysFailFast()
  }

  triggers {
    cron(env.BRANCH_NAME == 'master' ? '@daily' : '')
  }

  stages {
    stage('Precheck') {
      when {
        changeRequest target: 'stable'
      }
      steps {
        error "It is not allowed to create pull requests towards the 'stable' branch. Create a new pull request towards the 'master' branch please."
      }
    }
    stage('Build') {
      agent {
        docker {
          image 'prefec2/kieker-c-build'
          alwaysPull false
          args env.DOCKER_ARGS
        }
      }
      stages {
        stage('Initial Setup') {
          steps {
            // Make sure that no remainders from previous builds interfere.
            dir('source') {
              sh 'libtoolize'
              sh 'aclocal'
              sh 'autoconf'
              sh 'automake --add-missing'
            }
          }
        }

        stage('Compile') {
          steps {
            dir('source') {
              sh './configure'
              sh 'make'
            }
          }
        }

        stage('Static Analysis') {
          steps {
            sh 'echo dummy'
          }
        }
        
        stage('Unit Test') {
          steps {
            dir('source') {
              sh 'make check'
            }
          }
        }

        stage('Distribution Build') {
          steps {
            sh './create-debian.sh'
          }
        }
      }
    }
  }
}

